/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

#include <Eigen3x3.h>

//-----------------------------------------------------------------------------
inline void dsytrd3(double A[3][3], double Q[3][3], double d[3], double e[2])
//-----------------------------------------------------------------------------
// Reduces a symmetric 3x3 matrix to tridiagonal form by applying
// (unitary) Householder transformations:
//            [ d[0]  e[0]       ]
//    A = Q . [ e[0]  d[1]  e[1] ] . Q^T
//            [       e[1]  d[2] ]
// The function accesses only the diagonal and upper triangular parts of
// A. The access is read-only.
// ---------------------------------------------------------------------------
{
  const int n = 3;
  double u[n], q[n];
  double omega, f;
  double K, h, g;

  // Initialize Q to the identitity matrix
  for (int i = 0; i < n; i++)
  {
    Q[i][i] = 1.0;
    for (int j = 0; j < i; j++)
      Q[i][j] = Q[j][i] = 0.0;
  }

  // Bring first row and column to the desired form
  h = dnlSquare(A[0][1]) + dnlSquare(A[0][2]);
  if (A[0][1] > 0)
    g = -sqrt(h);
  else
    g = sqrt(h);
  e[0] = g;
  f = g * A[0][1];
  u[1] = A[0][1] - g;
  u[2] = A[0][2];

  omega = h - f;
  if (omega > 0.0)
  {
    omega = 1.0 / omega;
    K = 0.0;
    for (int i = 1; i < n; i++)
    {
      f = A[1][i] * u[1] + A[i][2] * u[2];
      q[i] = omega * f; // p
      K += u[i] * f;    // u* A u
    }
    K *= 0.5 * dnlSquare(omega);

    for (int i = 1; i < n; i++)
      q[i] = q[i] - K * u[i];

    d[0] = A[0][0];
    d[1] = A[1][1] - 2.0 * q[1] * u[1];
    d[2] = A[2][2] - 2.0 * q[2] * u[2];

    // Store inverse Householder transformation in Q
    for (int j = 1; j < n; j++)
    {
      f = omega * u[j];
      for (int i = 1; i < n; i++)
        Q[i][j] = Q[i][j] - f * u[i];
    }

    // Calculate updated A[1][2] and store it in e[1]
    e[1] = A[1][2] - q[1] * u[2] - u[1] * q[2];
  }
  else
  {
    for (int i = 0; i < n; i++)
      d[i] = A[i][i];
    e[1] = A[1][2];
  }
}

// ----------------------------------------------------------------------------
int dsyevq3(double A[3][3], double Q[3][3], double w[3])
//-----------------------------------------------------------------------------
// Calculates the eigenvalues and normalized eigenvectors of a symmetric 3x3
// matrix A using the QL algorithm with implicit shifts, preceded by a
// Householder reduction to tridiagonal form.
// The function accesses only the diagonal and upper triangular parts of A.
// The access is read-only.
//-----------------------------------------------------------------------------
// Parameters:
//   A: The symmetric input matrix
//   Q: Storage buffer for eigenvectors
//   w: Storage buffer for eigenvalues
//-----------------------------------------------------------------------------
// Return value:
//   0: Success
//  -1: Error (no convergence)
//-----------------------------------------------------------------------------
// Dependencies:
//   dsytrd3()
//-----------------------------------------------------------------------------
{
  const int n = 3;
  double e[3];                   // The third element is used only as temporary workspace
  double g, r, p, f, b, s, c, t; // Intermediate storage
  int nIter;
  int m;

  // Transform A to real tridiagonal form by the Householder method
  dsytrd3(A, Q, w, e);

  // Calculate eigensystem of the remaining real symmetric tridiagonal matrix
  // with the QL method
  //
  // Loop over all off-diagonal elements
  for (int l = 0; l < n - 1; l++)
  {
    nIter = 0;
    while (1)
    {
      // Check for convergence and exit iteration loop if off-diagonal
      // element e(l) is zero
      for (m = l; m <= n - 2; m++)
      {
        g = fabs(w[m]) + fabs(w[m + 1]);
        if (fabs(e[m]) + g == g)
          break;
      }
      if (m == l)
        break;

      if (nIter++ >= 30)
        return -1;

      // Calculate g = d_m - k
      g = (w[l + 1] - w[l]) / (e[l] + e[l]);
      r = sqrt(dnlSquare(g) + 1.0);
      if (g > 0)
        g = w[m] - w[l] + e[l] / (g + r);
      else
        g = w[m] - w[l] + e[l] / (g - r);

      s = c = 1.0;
      p = 0.0;
      for (int i = m - 1; i >= l; i--)
      {
        f = s * e[i];
        b = c * e[i];
        if (fabs(f) > fabs(g))
        {
          c = g / f;
          r = sqrt(dnlSquare(c) + 1.0);
          e[i + 1] = f * r;
          c *= (s = 1.0 / r);
        }
        else
        {
          s = f / g;
          r = sqrt(dnlSquare(s) + 1.0);
          e[i + 1] = g * r;
          s *= (c = 1.0 / r);
        }

        g = w[i + 1] - p;
        r = (w[i] - g) * s + 2.0 * c * b;
        p = s * r;
        w[i + 1] = g + p;
        g = c * r - b;

        // Form eigenvectors
        for (int k = 0; k < n; k++)
        {
          t = Q[k][i + 1];
          Q[k][i + 1] = s * Q[k][i] + c * t;
          Q[k][i] = c * Q[k][i] - s * t;
        }
      }
      w[l] -= p;
      e[l] = g;
      e[m] = 0.0;
    }
  }
  return 0;
}

// ----------------------------------------------------------------------------
inline void slvsec3(double d[3], double z[3], double w[3],
                    double R[3][3], int i0, int i1, int i2)
// ----------------------------------------------------------------------------
// Finds the three roots w_j of the secular equation
//   f(w_j) = 1 + Sum[ z_i / (d_i - w_j) ]  ==  0.
// It is assumed that d_0 <= d_1 <= d_2, and that all z_i have the same sign.
// The arrays P_i will contain the information required for the calculation
// of the eigenvectors:
//   P_ij = d_i - w_j.
// These differences can be obtained with better accuracy from intermediate
// results.
// ----------------------------------------------------------------------------
{
  double a[4];                    // Bounds of the intervals bracketing the roots
  double delta;                   // Shift of the d_i which ensures better accuracy
  double dd[3];                   // Shifted coefficients dd_i = d_i - delta
  double xl, xh;                  // Interval which straddles the current root. f(xl) < 0, f(xh) > 0
  double x;                       // Current estimates for the root
  double x0[3];                   // Analytically calculated roots, used as starting values
  double F, dF;                   // Function value f(x) and derivative f'(x)
  double dx, dxold;               // Current and last stepsizes
  double error;                   // Numerical error estimate, used for termination condition
  double t[3];                    // Temporary storage used for evaluating f
  double alpha, beta, gamma;      // Coefficients of polynomial f(x) * Product [ d_i - x ]
  double p, sqrt_p, q, c, s, phi; // Intermediate results of analytical calculation

  // Determine intervals which must contain the roots
  if (z[0] > 0)
  {
    a[0] = d[i0];
    a[1] = d[i1];
    a[2] = d[i2];
    a[3] = fabs(d[0] + 3.0 * z[0]) + fabs(d[1] + 3.0 * z[1]) + fabs(d[2] + 3.0 * z[2]);
  }
  else
  {
    a[0] = -fabs(d[0] + 3.0 * z[0]) - fabs(d[1] + 3.0 * z[1]) - fabs(d[2] + 3.0 * z[2]);
    a[1] = d[i0];
    a[2] = d[i1];
    a[3] = d[i2];
  }

  // Calculate roots of f(x) = 0 analytically (analogous to ZHEEVC3)
  t[0] = d[1] * d[2];
  t[1] = d[0] * d[2];
  t[2] = d[0] * d[1];
  gamma = t[0] * d[0] + (z[0] * t[0] + z[1] * t[1] + z[2] * t[2]); // Coefficients
  beta = (z[0] * (d[1] + d[2]) + z[1] * (d[0] + d[2]) + z[2] * (d[0] + d[1])) + (t[0] + t[1] + t[2]);
  alpha = (z[0] + z[1] + z[2]) + (d[0] + d[1] + d[2]);

  p = dnlSquare(alpha) - 3.0 * beta; // Transformation that removes the x^2 term
  q = alpha * (p - (3.0 / 2.0) * beta) + (27.0 / 2.0) * gamma;
  sqrt_p = sqrt(fabs(p));

  phi = 27.0 * (0.25 * dnlSquare(beta) * (p - beta) - gamma * (q - 27.0 / 4.0 * gamma));
  phi = (1.0 / 3.0) * atan2(sqrt(fabs(phi)), q);
  c = sqrt_p * cos(phi);
  s = (1.0 / dnlSqrt3) * sqrt_p * fabs(sin(phi));

  x0[0] = x0[1] = x0[2] = (1.0 / 3.0) * (alpha - c);
  if (c > s) // Make sure the roots are in ascending order
  {
    x0[0] -= s;
    x0[1] += s;
    x0[2] += c;
  }
  else if (c < -s)
  {
    x0[0] += c;
    x0[1] -= s;
    x0[2] += s;
  }
  else
  {
    x0[0] -= s;
    x0[1] += c;
    x0[2] += s;
  }

  // Refine roots with a combined Bisection/Newton-Raphson method
  for (int i = 0; i < 3; i++)
  {
    xl = a[i];     // Lower bound of bracketing interval
    xh = a[i + 1]; // Upper bound of bracketing interval
    dx = dxold = 0.5 * (xh - xl);

    // Make sure that xl != xh
    if (dx == 0.0)
    {
      w[i] = xl;
      for (int j = 0; j < 3; j++)
        R[j][i] = d[j] - xl;
      continue;
    }

    // Shift the root close to zero to achieve better accuracy
    if (x0[i] >= xh)
    {
      delta = xh;
      x = -dx;
      for (int j = 0; j < 3; j++)
      {
        dd[j] = d[j] - delta;
        R[j][i] = dd[j] - x;
      }
    }
    else if (x0[i] <= xl)
    {
      delta = xl;
      x = dx;
      for (int j = 0; j < 3; j++)
      {
        dd[j] = d[j] - delta;
        R[j][i] = dd[j] - x;
      }
    }
    else
    {
      delta = x0[i];
      x = 0.0;
      for (int j = 0; j < 3; j++)
        R[j][i] = dd[j] = d[j] - delta;
    }
    xl -= delta;
    xh -= delta;

    // Make sure that f(xl) < 0 and f(xh) > 0
    if (z[0] < 0.0)
    {
      double t = xh;
      xh = xl;
      xl = t;
    }

    // Main iteration loop
    for (int nIter = 0; nIter < 500; nIter++)
    {
      // Evaluate f and f', and calculate an error estimate
      F = 1.0;
      dF = 0.0;
      error = 1.0;
      for (int j = 0; j < 3; j++)
      {
        t[0] = 1.0 / R[j][i];
        t[1] = z[j] * t[0];
        t[2] = t[1] * t[0];
        F += t[1];
        error += fabs(t[1]);
        dF += t[2];
      }

      // Check for convergence
      if (fabs(F) <= dnlEpsilon * (8.0 * error + fabs(x * dF)))
        break;

      // Adjust interval boundaries
      if (F < 0.0)
        xl = x;
      else
        xh = x;

      // Check, whether Newton-Raphson would converge fast enough. If so,
      // give it a try. If not, or if it would run out of bounds, use bisection
      if (fabs(2.0 * F) < fabs(dxold * dF))
      {
        dxold = dx;
        dx = F / dF;
        x = x - dx;
        if ((x - xh) * (x - xl) >= 0.0)
        {
          dx = 0.5 * (xh - xl);
          x = xl + dx;
        }
      }
      else
      {
        dx = 0.5 * (xh - xl);
        x = xl + dx;
      }

      // Prepare next iteration
      for (int j = 0; j < 3; j++)
        R[j][i] = dd[j] - x;
    }

    // Un-shift result
    w[i] = x + delta;
  }
}

// ----------------------------------------------------------------------------
inline void dsyev2(double A, double B, double C, double *rt1, double *rt2,
                   double *cs, double *sn)
// ----------------------------------------------------------------------------
// Calculates the eigensystem of a real symmetric 2x2 matrix
//    [ A  B ]
//    [ B  C ]
// in the form
//    [ A  B ]  =  [ cs  -sn ] [ rt1   0  ] [  cs  sn ]
//    [ B  C ]     [ sn   cs ] [  0   rt2 ] [ -sn  cs ]
// where rt1 >= rt2. Note that this convention is different from the one used
// in the LAPACK routine DLAEV2, where |rt1| >= |rt2|.
// ----------------------------------------------------------------------------
{
  double sm = A + C;
  double df = A - C;
  double rt = sqrt(dnlSquare(df) + 4.0 * B * B);
  double t;

  if (sm > 0.0)
  {
    *rt1 = 0.5 * (sm + rt);
    t = 1.0 / (*rt1);
    *rt2 = (A * t) * C - (B * t) * B;
  }
  else if (sm < 0.0)
  {
    *rt2 = 0.5 * (sm - rt);
    t = 1.0 / (*rt2);
    *rt1 = (A * t) * C - (B * t) * B;
  }
  else // This case needs to be treated separately to avoid div by 0
  {
    *rt1 = 0.5 * rt;
    *rt2 = -0.5 * rt;
  }

  // Calculate eigenvectors
  if (df > 0.0)
    *cs = df + rt;
  else
    *cs = df - rt;

  if (fabs(*cs) > 2.0 * fabs(B))
  {
    t = -2.0 * B / *cs;
    *sn = 1.0 / sqrt(1.0 + dnlSquare(t));
    *cs = t * (*sn);
  }
  else if (fabs(B) == 0.0)
  {
    *cs = 1.0;
    *sn = 0.0;
  }
  else
  {
    t = -0.5 * (*cs) / B;
    *cs = 1.0 / sqrt(1.0 + dnlSquare(t));
    *sn = t * (*cs);
  }

  if (df > 0.0)
  {
    t = *cs;
    *cs = -(*sn);
    *sn = t;
  }
}

// ----------------------------------------------------------------------------
int dsyevd3(double A[3][3], double Q[3][3], double w[3])
// ----------------------------------------------------------------------------
// Calculates the eigenvalues and normalized eigenvectors of a symmetric 3x3
// matrix A using Cuppen's Divide & Conquer algorithm.
// The function accesses only the diagonal and upper triangular parts of A.
// The access is read-only.
// ----------------------------------------------------------------------------
// Parameters:
//   A: The symmetric input matrix
//   Q: Storage buffer for eigenvectors
//   w: Storage buffer for eigenvalues
// ----------------------------------------------------------------------------
// Return value:
//   0: Success
//  -1: Error
// ----------------------------------------------------------------------------
// Dependencies:
//   dsyev2(), slvsec3(), dsytrd3()
// ----------------------------------------------------------------------------
{
  const int n = 3;
  double R[3][3]; // Householder transformation matrix
  double P[3][3]; // Unitary transformation matrix which diagonalizes D + w w^T
  double e[2];    // Off-diagonal elements after Householder transformation
  double d[3];    // Eigenvalues of split matrix in the "divide" step)
  double c, s;    // Eigenvector of 2x2 block in the "divide" step
  double z[3];    // Numerators of secular equation / Updating vector
  double t;       // Miscellaenous temporary stuff

  // Initialize Q
  memset(Q, 0.0, 9 * sizeof(double));

  // Transform A to real tridiagonal form by the Householder method
  dsytrd3(A, R, w, e);

  // "Divide"
  // --------

  // Detect matrices that factorize to avoid multiple eigenvalues in the Divide/Conquer algorithm
  for (int i = 0; i < n - 1; i++)
  {
    t = fabs(w[i]) + fabs(w[i + 1]);
    if (fabs(e[i]) <= 8.0 * dnlEpsilon * t)
    {
      if (i == 0)
      {
        dsyev2(w[1], e[1], w[2], &d[1], &d[2], &c, &s);
        w[1] = d[1];
        w[2] = d[2];
        Q[0][0] = 1.0;
        for (int j = 1; j < n; j++)
        {
          Q[j][1] = s * R[j][2] + c * R[j][1];
          Q[j][2] = c * R[j][2] - s * R[j][1];
        }
      }
      else
      {
        dsyev2(w[0], e[0], w[1], &d[0], &d[1], &c, &s);
        w[0] = d[0];
        w[1] = d[1];
        Q[0][0] = c;
        Q[0][1] = -s;
        Q[1][0] = R[1][1] * s;
        Q[1][1] = R[1][1] * c;
        Q[1][2] = R[1][2];
        Q[2][0] = R[2][1] * s;
        Q[2][1] = R[2][1] * c;
        Q[2][2] = R[2][2];
      }

      return 0;
    }
  }

  // Calculate eigenvalues and eigenvectors of 2x2 block
  dsyev2(w[1] - e[0], e[1], w[2], &d[1], &d[2], &c, &s);
  d[0] = w[0] - e[0];

  // "Conquer"
  // ---------

  // Determine coefficients of secular equation
  z[0] = e[0];
  z[1] = e[0] * dnlSquare(c);
  z[2] = e[0] * dnlSquare(s);

  // Call slvsec3 with d sorted in ascending order. We make
  // use of the fact that dsyev2 guarantees d[1] >= d[2].
  if (d[0] < d[2])
    slvsec3(d, z, w, P, 0, 2, 1);
  else if (d[0] < d[1])
    slvsec3(d, z, w, P, 2, 0, 1);
  else
    slvsec3(d, z, w, P, 2, 1, 0);

  // Calculate eigenvectors of matrix D + beta * z * z^t and store them in the
  // columns of P
  z[0] = sqrt(fabs(e[0]));
  z[1] = c * z[0];
  z[2] = -s * z[0];

  // Detect duplicate elements in d to avoid division by zero
  t = 8.0 * dnlEpsilon * (fabs(d[0]) + fabs(d[1]) + fabs(d[2]));
  if (fabs(d[1] - d[0]) <= t)
  {
    for (int j = 0; j < n; j++)
    {
      if (P[0][j] * P[1][j] <= 0.0)
      {
        P[0][j] = z[1];
        P[1][j] = -z[0];
        P[2][j] = 0.0;
      }
      else
        for (int i = 0; i < n; i++)
          P[i][j] = z[i] / P[i][j];
    }
  }
  else if (fabs(d[2] - d[0]) <= t)
  {
    for (int j = 0; j < n; j++)
    {
      if (P[0][j] * P[2][j] <= 0.0)
      {
        P[0][j] = z[2];
        P[1][j] = 0.0;
        P[2][j] = -z[0];
      }
      else
        for (int i = 0; i < n; i++)
          P[i][j] = z[i] / P[i][j];
    }
  }
  else
  {
    for (int j = 0; j < n; j++)
      for (int i = 0; i < n; i++)
      {
        if (P[i][j] == 0.0)
        {
          P[i][j] = 1.0;
          P[(i + 1) % n][j] = 0.0;
          P[(i + 2) % n][j] = 0.0;
          break;
        }
        else
          P[i][j] = z[i] / P[i][j];
      }
  }

  // Normalize eigenvectors of D + beta * z * z^t
  for (int j = 0; j < n; j++)
  {
    t = dnlSquare(P[0][j]) + dnlSquare(P[1][j]) + dnlSquare(P[2][j]);
    t = 1.0 / sqrt(t);
    for (int i = 0; i < n; i++)
      P[i][j] *= t;
  }

  // Undo diagonalization of 2x2 block
  for (int j = 0; j < n; j++)
  {
    t = P[1][j];
    P[1][j] = c * t - s * P[2][j];
    P[2][j] = s * t + c * P[2][j];
  }

  // Undo Householder transformation
  for (int j = 0; j < n; j++)
    for (int k = 0; k < n; k++)
    {
      t = P[k][j];
      for (int i = 0; i < n; i++)
        Q[i][j] += t * R[i][k];
    }

  return 0;
}

// ----------------------------------------------------------------------------
int dsyevj3(double A[3][3], double Q[3][3], double w[3])
// ----------------------------------------------------------------------------
// Calculates the eigenvalues and normalized eigenvectors of a symmetric 3x3
// matrix A using the Jacobi algorithm.
// The upper triangular part of A is destroyed during the calculation,
// the diagonal elements are read but not destroyed, and the lower
// triangular elements are not referenced at all.
// ----------------------------------------------------------------------------
// Parameters:
//   A: The symmetric input matrix
//   Q: Storage buffer for eigenvectors
//   w: Storage buffer for eigenvalues
// ----------------------------------------------------------------------------
// Return value:
//   0: Success
//  -1: Error (no convergence)
// ----------------------------------------------------------------------------
{
  const int n = 3;
  double sd, so;         // Sums of diagonal resp. off-diagonal elements
  double s, c, t;        // sin(phi), cos(phi), tan(phi) and temporary storage
  double g, h, z, theta; // More temporary storage
  double thresh;

  // Initialize Q to the identitity matrix
  for (int i = 0; i < n; i++)
  {
    Q[i][i] = 1.0;
    for (int j = 0; j < i; j++)
      Q[i][j] = Q[j][i] = 0.0;
  }

  // Initialize w to diag(A)
  for (int i = 0; i < n; i++)
    w[i] = A[i][i];

  // Calculate dnlSquare(tr(A))
  sd = 0.0;
  for (int i = 0; i < n; i++)
    sd += fabs(w[i]);
  sd = dnlSquare(sd);

  // Main iteration loop
  for (int nIter = 0; nIter < 50; nIter++)
  {
    // Test for convergence
    so = 0.0;
    for (int p = 0; p < n; p++)
      for (int q = p + 1; q < n; q++)
        so += fabs(A[p][q]);
    if (so == 0.0)
      return 0;

    if (nIter < 4)
      thresh = 0.2 * so / dnlSquare(n);
    else
      thresh = 0.0;

    // Do sweep
    for (int p = 0; p < n; p++)
      for (int q = p + 1; q < n; q++)
      {
        g = 100.0 * fabs(A[p][q]);
        if (nIter > 4 && fabs(w[p]) + g == fabs(w[p]) && fabs(w[q]) + g == fabs(w[q]))
        {
          A[p][q] = 0.0;
        }
        else if (fabs(A[p][q]) > thresh)
        {
          // Calculate Jacobi transformation
          h = w[q] - w[p];
          if (fabs(h) + g == fabs(h))
          {
            t = A[p][q] / h;
          }
          else
          {
            theta = 0.5 * h / A[p][q];
            if (theta < 0.0)
              t = -1.0 / (sqrt(1.0 + dnlSquare(theta)) - theta);
            else
              t = 1.0 / (sqrt(1.0 + dnlSquare(theta)) + theta);
          }
          c = 1.0 / sqrt(1.0 + dnlSquare(t));
          s = t * c;
          z = t * A[p][q];

          // Apply Jacobi transformation
          A[p][q] = 0.0;
          w[p] -= z;
          w[q] += z;
          for (int r = 0; r < p; r++)
          {
            t = A[r][p];
            A[r][p] = c * t - s * A[r][q];
            A[r][q] = s * t + c * A[r][q];
          }
          for (int r = p + 1; r < q; r++)
          {
            t = A[p][r];
            A[p][r] = c * t - s * A[r][q];
            A[r][q] = s * t + c * A[r][q];
          }
          for (int r = q + 1; r < n; r++)
          {
            t = A[p][r];
            A[p][r] = c * t - s * A[q][r];
            A[q][r] = s * t + c * A[q][r];
          }

          // Update eigenvectors
          for (int r = 0; r < n; r++)
          {
            t = Q[r][p];
            Q[r][p] = c * t - s * Q[r][q];
            Q[r][q] = s * t + c * Q[r][q];
          }
        }
      }
  }
  return -1;
}
