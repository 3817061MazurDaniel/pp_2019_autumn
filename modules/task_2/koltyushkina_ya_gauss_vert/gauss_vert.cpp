// Copyright 2019 Koltyushkina Yanina
#include "../../../modules/task_2/koltyushkina_ya_gauss_vert/gauss_vert.h"
#include <mpi.h>
#include <cmath>
#include <random>
#include <vector>

std::vector <double> RandomMatrix(int _size) {
  std::mt19937 seed;
  if (_size <= 0)
    throw "Zero or negative size";
  std::vector <double> mtr(_size*(_size + 1));
  for (int i = 0; i < _size*(_size + 1); i++) {
    mtr[i] = seed();
  }
  return mtr;
}

int maxind(int it, int _size, std::vector <double> mtr) {
  double max = std::abs(mtr[it*_size + it]);
  int in = it;
  for (int i = it; i < _size; i++) {
    if (std::abs(mtr[it*_size + i]) > max) {
      max = std::abs(mtr[it*_size + i]);
      in = i;
    }
  }
  return in;
}

std::vector<double> PrGauss(std::vector<double> mtr, int _size) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int segm = (_size + 1) / size;
  int ost = (_size + 1) % size;

  std::vector <double> maskoef(_size);
  std::vector <double> tmpmaskoef(_size, 1);
  std::vector <double> prom_res(_size*(_size + 1));
  prom_res = mtr;

  MPI_Status status;
  maskoef[0] = 1;
  int maxin;
  std::vector <double> tmp(_size + 1);
  int f = 1;
  int krch;
  for (int it = 0; it < _size - 1; it++) {
    maxin = maxind(it, _size, mtr);
    for (int i = 0; i < _size + 1; i++) {
      tmp[i] = mtr[i*_size + it];
      mtr[i*_size + it] = mtr[i*_size + maxin];
      mtr[i*_size + maxin] = tmp[i];
    }
    for (int i = 0; i < _size + 1; i++) {
      tmp[i] = prom_res[i*_size + it];
      prom_res[i*_size + it] = prom_res[i*_size + maxin];
      prom_res[i*_size + maxin] = tmp[i];
    }
    if (rank == 0) {
      for (int i = it + 1; i < _size; i++) {
        maskoef[i] = mtr[it*_size + i] / mtr[it*_size + it];
      }
      for (int i = 1; i < size; i++) {
        MPI_Send(&maskoef[f], _size - (it + 1), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      }
      f++;
    } else {
      MPI_Recv(&tmpmaskoef[1], _size - (it + 1), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }
    if (rank == 0) {
      for (int i = 0; i < segm; i++) {
        for (int j = it + 1; j < _size; j++) {
          mtr[(i*size + rank)*_size + j] -= maskoef[j] * mtr[(i*size + rank)*_size + it];
        }
        for (int k = 1; k < size; k++) {
          MPI_Recv(&mtr[(i*size + k)*_size + it + 1], _size - it - 1, MPI_DOUBLE, k, 9, MPI_COMM_WORLD, &status);
        }
      }
      for (int i = 0; i < ost; i++) {
        for (int j = it + 1; j < _size; j++) {
          mtr[(segm*size + i)*_size + j] -= maskoef[j] * mtr[(segm*size + i)*_size + it];
        }
      }
    } else {
      for (int i = 0; i < segm; i++) {
        f = 1;
        for (int j = it + 1; j < _size; j++) {
          krch = i * size + rank;
          prom_res[(krch)*_size + j] = prom_res[(krch)*_size + j] - tmpmaskoef[f] * prom_res[(krch)*_size + it];
          f++;
        }
        MPI_Send(&prom_res[(i*size + rank)*_size + it + 1], _size - it - 1, MPI_DOUBLE, 0, 9, MPI_COMM_WORLD);
      }
    }
  }
  return mtr;
}
bool NullStr(std::vector<double> mtr, int str, int _size) {
  for (int i = str; i < _size; i++)
    if (mtr[i*_size + str] != 0)
      return 0;
  return 1;
}

int Proverka(std::vector<double> mtr, int _size) {
  for (int i = 0; i < _size; i++) {
    if (NullStr(mtr, i, _size) == 1) {
      if (mtr[(_size + 1)*_size + i] == 0)
        return i;
      if (mtr[(_size + 1)*_size + i] != 0)
        throw "There are no solutions";
    }
  }
  return 0;
}

std::vector<double> ObrGauss(std::vector<double> mtr, int _size) {
  int size, rank;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> dres(_size);
  std::vector<double> b(_size, 1);
  int segm, ost;
  int prov = Proverka(mtr, _size);
  if (prov != 0)
    throw "Vector is not double";
  if (prov == 0) {
    segm = _size / size;
    ost = _size % size;
    int fk = 0;
    double koef;
    for (int i = _size - 1; i > 0; i--) {
      fk = 0;
      if ((rank != 0) && (i < segm*size) && (rank == i % size)) {
        MPI_Send(&mtr[_size*_size + i], 1, MPI_DOUBLE, 0, i, MPI_COMM_WORLD);
      }

      if ((i%size != 0) && (i < segm*size) && (rank == 0)) {
        MPI_Recv(&b[i], 1, MPI_DOUBLE, i%size, i, MPI_COMM_WORLD, &status);
        fk = 1;
      }
      if ((fk == 0) && (rank == 0)) {
        b[i] = mtr[_size*_size + i];
      }
      if (rank == 0) {
        dres[i] = b[i] / mtr[_size*i + i];
        koef = dres[i];
      }
      MPI_Bcast(&koef, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      for (int s = 0; s < segm; s++) {
        mtr[_size*_size + s * size + rank] -= koef * mtr[i*_size + s * size + rank];
      }
      if (rank == 0) {
        for (int s = 0; s < ost; s++) {
          mtr[_size*_size + segm * size + s] -= koef * mtr[i*_size + segm * size + s];
        }
      }
    }
    if (rank == 0) {
      dres[0] = mtr[_size * _size + 0] / mtr[_size * 0 + 0];
    }
  }
  return dres;
}
std::vector<double> All(std::vector<double> mtr, int _size) {
  std::vector<double> mtr0(_size*(_size + 1));
  std::vector<double> dres(_size);
  mtr0 = PrGauss(mtr, _size);
  dres = ObrGauss(mtr0, _size);
  return dres;
}
