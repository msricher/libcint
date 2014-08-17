/*
 * File: cart2sph.c
 * Author: Qiming Sun <osirpt.sun@gmail.com>
 *
 * Cartisen GTO to spheric or spinor GTO transformation
 * (and reorder the integrals from block(i,j,k,l)_{contr_idx(I,K,L,J)}
 * to (iI,jJ,kK,lL))
 */

#include <stdlib.h>
#include <complex.h>
#include "cint_const.h"
#include "cint_bas.h"
#include "cart2sph.h"
#include "misc.h"


/*
 * Cartesian to real-spheric coefficients
 * ref: H. B. Schlegel and M. J.  Frisch, Int. J. Quant. Chem., 54(1995), 83-87.
 * different from the usual definition such as
 *   p_-1 p_0 p_1 => (px, py, pz) ...
 * the map here is
 *   p_-1 p_0 p_1 => (py, pz, px) ...
 *
void fill_cart2sph(const int l, double *t)
{
        int m, lx, ly, lz;
        for (m = -l; m <= l; m++) {
                for (lx = l; lx >= 0; lx--) {
                        for (ly = *lmax - lx; ly >= 0; ly--) {
                                lz = *lmax - lx - ly;
                                *t++ = dxyzfactor_real_spheric_UPI_(&lx, &ly, &lz, &m);
                        }
                }
        }
}
 */

static const double g_trans_cart2sph[476] = {
        1, /* factors of s and p are moved to CINTcommon_fac_sp */
        // px
        1,
        0,
        0,
        // py
        0,
        1,
        0,
        // pz
        0,
        0,
        1,
        // dxy
        0,
        1.092548430592079070,
        0,
        0,
        0,
        0,
        // dyz
        0,
        0,
        0,
        0,
        1.092548430592079070,
        0,
        // dz2
        -0.315391565252520002,
        0,
        0,
        -0.315391565252520002,
        0,
        0.630783130505040012,
        // dxz
        0,
        0,
        1.092548430592079070,
        0,
        0,
        0,
        // dy2
        0.546274215296039535,
        0,
        0,
        -0.546274215296039535,
        0,
        0,
        // f-3 ~ fyx2
        0,
        1.770130769779930531,
        0,
        0,
        0,
        0,
        -0.590043589926643510,
        0,
        0,
        0,
        // f-2 ~ fxyz
        0,
        0,
        0,
        0,
        2.890611442640554055,
        0,
        0,
        0,
        0,
        0,
        // f-1 ~ fyz2
        0,
        -0.457045799464465739,
        0,
        0,
        0,
        0,
        -0.457045799464465739,
        0,
        1.828183197857862944,
        0,
        // f0 ~ fz3
        0,
        0,
        -1.119528997770346170,
        0,
        0,
        0,
        0,
        -1.119528997770346170,
        0,
        0.746352665180230782,
        // f1 ~ fxz2
        -0.457045799464465739,
        0,
        0,
        -0.457045799464465739,
        0,
        1.828183197857862944,
        0,
        0,
        0,
        0,
        // f2 ~ fzx2
        0,
        0,
        1.445305721320277020,
        0,
        0,
        0,
        0,
        -1.445305721320277020,
        0,
        0,
        // f3 ~ fx3
        0.590043589926643510,
        0,
        0,
        -1.770130769779930530,
        0,
        0,
        0,
        0,
        0,
        0,
        // g-4 ~ gyx3
        0,
        2.503342941796704538,
        0,
        0,
        0,
        0,
        -2.503342941796704530,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        // g-3 ~ gx2yz
        0,
        0,
        0,
        0,
        5.310392309339791593,
        0,
        0,
        0,
        0,
        0,
        0,
        -1.770130769779930530,
        0,
        0,
        0,
        // g-2 ~ gxyz2
        0,
        -0.946174695757560014,
        0,
        0,
        0,
        0,
        -0.946174695757560014,
        0,
        5.677048174545360108,
        0,
        0,
        0,
        0,
        0,
        0,
        // g-1 ~ gyz3
        0,
        0,
        0,
        0,
        -2.007139630671867500,
        0,
        0,
        0,
        0,
        0,
        0,
        -2.007139630671867500,
        0,
        2.676186174229156671,
        0,
        // g0 ~ gz4
        0.317356640745612911,
        0,
        0,
        0.634713281491225822,
        0,
        -2.538853125964903290,
        0,
        0,
        0,
        0,
        0.317356640745612911,
        0,
        -2.538853125964903290,
        0,
        0.846284375321634430,
        // g1 ~ gxz3
        0,
        0,
        -2.007139630671867500,
        0,
        0,
        0,
        0,
        -2.007139630671867500,
        0,
        2.676186174229156671,
        0,
        0,
        0,
        0,
        0,
        // g2 ~ gx2z2
        -0.473087347878780002,
        0,
        0,
        0,
        0,
        2.838524087272680054,
        0,
        0,
        0,
        0,
        0.473087347878780009,
        0,
        -2.838524087272680050,
        0,
        0,
        // g3 ~ gzx3
        0,
        0,
        1.770130769779930531,
        0,
        0,
        0,
        0,
        -5.310392309339791590,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        // g4 ~ gy4
        0.625835735449176134,
        0,
        0,
        -3.755014412695056800,
        0,
        0,
        0,
        0,
        0,
        0,
        0.625835735449176134,
        0,
        0,
        0,
        0,
        // h-5 ~ hyx4
        0,
        3.281910284200850514,
        0,
        0,
        0,
        0,
        -6.563820568401701020,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0.656382056840170102,
        0,
        0,
        0,
        0,
        0,
        // h-4 ~ hx3yz
        0,
        0,
        0,
        0,
        8.302649259524165115,
        0,
        0,
        0,
        0,
        0,
        0,
        -8.302649259524165110,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        // h-3 ~ hyx2z2
        0,
        -1.467714898305751160,
        0,
        0,
        0,
        0,
        -0.978476598870500779,
        0,
        11.741719186446009300,
        0,
        0,
        0,
        0,
        0,
        0,
        0.489238299435250387,
        0,
        -3.913906395482003100,
        0,
        0,
        0,
        // h-2 ~ hxyz3
        0,
        0,
        0,
        0,
        -4.793536784973323750,
        0,
        0,
        0,
        0,
        0,
        0,
        -4.793536784973323750,
        0,
        9.587073569946647510,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        // h-1 ~ hyz4
        0,
        0.452946651195696921,
        0,
        0,
        0,
        0,
        0.905893302391393842,
        0,
        -5.435359814348363050,
        0,
        0,
        0,
        0,
        0,
        0,
        0.452946651195696921,
        0,
        -5.435359814348363050,
        0,
        3.623573209565575370,
        0,
        // h0 ~ hx2y2z
        0,
        0,
        1.754254836801353946,
        0,
        0,
        0,
        0,
        3.508509673602707893,
        0,
        -4.678012898136943850,
        0,
        0,
        0,
        0,
        0,
        0,
        1.754254836801353946,
        0,
        -4.678012898136943850,
        0,
        0.935602579627388771,
        // h1 ~ xz4
        0.452946651195696921,
        0,
        0,
        0.905893302391393842,
        0,
        -5.435359814348363050,
        0,
        0,
        0,
        0,
        0.452946651195696921,
        0,
        -5.435359814348363050,
        0,
        3.623573209565575370,
        0,
        0,
        0,
        0,
        0,
        0,
        // h2 ~ hx2z3
        0,
        0,
        -2.396768392486661870,
        0,
        0,
        0,
        0,
        0,
        0,
        4.793536784973323755,
        0,
        0,
        0,
        0,
        0,
        0,
        2.396768392486661877,
        0,
        -4.793536784973323750,
        0,
        0,
        // h3 ~ hx3z2
        -0.489238299435250389,
        0,
        0,
        0.978476598870500775,
        0,
        3.913906395482003101,
        0,
        0,
        0,
        0,
        1.467714898305751163,
        0,
        -11.741719186446009300,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        // h4 ~ hzy4
        0,
        0,
        2.075662314881041278,
        0,
        0,
        0,
        0,
        -12.453973889286247600,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        2.075662314881041278,
        0,
        0,
        0,
        0,
        // h5 ~ hxy4
        0.656382056840170102,
        0,
        0,
        -6.563820568401701020,
        0,
        0,
        0,
        0,
        0,
        0,
        3.281910284200850514,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0};

/*
 * / xyz_alpha \
 * \ xyz_beta  /
 */
static const double complex g_trans_cart2j[1904] = {
        0 + 0*_Complex_I,
        1 + 0*_Complex_I,
        1 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.577350269189625764 + 0*_Complex_I,  // p1/2
        0 + 0.577350269189625764*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.577350269189625764 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.577350269189625764 + 0*_Complex_I,
       -0.577350269189625764 + 0*_Complex_I,
        0 - 0.577350269189625764*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,  // p3/2
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.707106781186547524 + 0*_Complex_I,
        0 - 0.707106781186547524*_Complex_I,
        0 + 0*_Complex_I,
        0.408248290463863016 + 0*_Complex_I,
        0 - 0.408248290463863016*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.816496580927726033 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.816496580927726033 + 0*_Complex_I,
       -0.408248290463863016 + 0*_Complex_I,
        0 - 0.408248290463863016*_Complex_I,
        0 + 0*_Complex_I,
       -0.707106781186547524 + 0*_Complex_I,
        0 - 0.707106781186547524*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.345494149471335479 + 0*_Complex_I,  // d3/2
        0 + 0.690988298942670958*_Complex_I,
        0 + 0*_Complex_I,
        0.345494149471335479 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.345494149471335479 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.345494149471335479*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.598413420602149016 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.598413420602149016*_Complex_I,
        0 + 0*_Complex_I,
       -0.199471140200716338 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.199471140200716338 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.398942280401432677 + 0*_Complex_I,
        0.199471140200716338 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.199471140200716338 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.398942280401432677 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.598413420602149016 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.598413420602149016*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.345494149471335479 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.345494149471335479*_Complex_I,
        0 + 0*_Complex_I,
        0.345494149471335479 + 0*_Complex_I,
        0 + 0.690988298942670958*_Complex_I,
        0 + 0*_Complex_I,
       -0.345494149471335479 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.386274202023189580 + 0*_Complex_I,
        0 - 0.772548404046379160*_Complex_I,
        0 + 0*_Complex_I,
       -0.386274202023189580 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.172747074735667739 + 0*_Complex_I,
        0 - 0.345494149471335479*_Complex_I,
        0 + 0*_Complex_I,
       -0.172747074735667739 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.690988298942670958 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.690988298942670958*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.488602511902919921 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.488602511902919921*_Complex_I,
        0 + 0*_Complex_I,
       -0.244301255951459960 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.244301255951459960 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.488602511902919921 + 0*_Complex_I,
       -0.244301255951459960 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.244301255951459960 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.488602511902919921 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.488602511902919921 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.488602511902919921*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.690988298942670958 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.690988298942670958*_Complex_I,
        0 + 0*_Complex_I,
        0.172747074735667739 + 0*_Complex_I,
        0 + 0.345494149471335479*_Complex_I,
        0 + 0*_Complex_I,
       -0.172747074735667739 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.386274202023189580 + 0*_Complex_I,
        0 + 0.772548404046379160*_Complex_I,
        0 + 0*_Complex_I,
       -0.386274202023189580 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.386274202023189580 + 0*_Complex_I,
        0 + 1.158822606069568741*_Complex_I,
        0 + 0*_Complex_I,
        1.158822606069568741 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.386274202023189580*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.386274202023189580 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.772548404046379160*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.386274202023189580 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.863735373678338698 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.727470747356677396*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.863735373678338698 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.172747074735667739 + 0*_Complex_I,
        0 + 0.172747074735667739*_Complex_I,
        0 + 0*_Complex_I,
       -0.172747074735667739 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.690988298942670958 + 0*_Complex_I,
        0 + 0.172747074735667739*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.690988298942670958*_Complex_I,
        0 + 0*_Complex_I,
        0.244301255951459960 + 0*_Complex_I,
        0 - 0.244301255951459960*_Complex_I,
        0 + 0*_Complex_I,
        0.244301255951459960 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.977205023805839843 + 0*_Complex_I,
        0 - 0.244301255951459960*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.977205023805839843*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.732903767854379882 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.732903767854379882 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.488602511902919921 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.732903767854379882 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.732903767854379882 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.488602511902919921 + 0*_Complex_I,
        0.244301255951459960 + 0*_Complex_I,
        0 + 0.244301255951459960*_Complex_I,
        0 + 0*_Complex_I,
        0.244301255951459960 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.977205023805839843 + 0*_Complex_I,
        0 + 0.244301255951459960*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.977205023805839843*_Complex_I,
        0 + 0*_Complex_I,
       -0.172747074735667739 + 0*_Complex_I,
        0 - 0.172747074735667739*_Complex_I,
        0 + 0*_Complex_I,
       -0.172747074735667739 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.690988298942670958 + 0*_Complex_I,
        0 - 0.172747074735667739*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.690988298942670958*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.863735373678338698 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.727470747356677396*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.863735373678338698 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.386274202023189580 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.772548404046379160*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.386274202023189580 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.386274202023189580 + 0*_Complex_I,
        0 - 1.158822606069568741*_Complex_I,
        0 + 0*_Complex_I,
        1.158822606069568741 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.386274202023189580*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.417223823632784089 + 0*_Complex_I,
        0 - 1.251671470898352269*_Complex_I,
        0 + 0*_Complex_I,
       -1.251671470898352269 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.417223823632784089*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.157695782626260003 + 0*_Complex_I,
        0 - 0.473087347878780009*_Complex_I,
        0 + 0*_Complex_I,
       -0.473087347878780009 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.157695782626260003*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.946174695757560018 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.892349391515120036*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.946174695757560018 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.546274215296039535 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.092548430592079070*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.546274215296039535 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.273137107648019767 + 0*_Complex_I,
        0 + 0.273137107648019767*_Complex_I,
        0 + 0*_Complex_I,
       -0.273137107648019767 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.092548430592079070 + 0*_Complex_I,
        0 + 0.273137107648019767*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.092548430592079070*_Complex_I,
        0 + 0*_Complex_I,
       -0.211571093830408607 + 0*_Complex_I,
        0 + 0.211571093830408607*_Complex_I,
        0 + 0*_Complex_I,
       -0.211571093830408607 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.846284375321634430 + 0*_Complex_I,
        0 + 0.211571093830408607*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.846284375321634430*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.846284375321634430 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.846284375321634430 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.564189583547756286 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.846284375321634430 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.846284375321634430 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.564189583547756286 + 0*_Complex_I,
        0.211571093830408607 + 0*_Complex_I,
        0 + 0.211571093830408607*_Complex_I,
        0 + 0*_Complex_I,
        0.211571093830408607 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.846284375321634430 + 0*_Complex_I,
        0 + 0.211571093830408607*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.846284375321634430*_Complex_I,
        0 + 0*_Complex_I,
        0.273137107648019767 + 0*_Complex_I,
        0 + 0.273137107648019767*_Complex_I,
        0 + 0*_Complex_I,
        0.273137107648019767 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.092548430592079070 + 0*_Complex_I,
        0 + 0.273137107648019767*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.092548430592079070*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.546274215296039535 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.092548430592079070*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.546274215296039535 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.946174695757560018 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.892349391515120036*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.946174695757560018 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.157695782626260003 + 0*_Complex_I,
        0 - 0.473087347878780009*_Complex_I,
        0 + 0*_Complex_I,
        0.473087347878780009 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.157695782626260003*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.417223823632784089 + 0*_Complex_I,
        0 - 1.251671470898352269*_Complex_I,
        0 + 0*_Complex_I,
        1.251671470898352269 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.417223823632784089*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.417223823632784089 + 0*_Complex_I,
        0 + 1.668895294531136358*_Complex_I,
        0 + 0*_Complex_I,
        2.503342941796704538 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.668895294531136358*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.417223823632784089 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.417223823632784089 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.251671470898352269*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.251671470898352269 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.417223823632784089*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.103870478383820021 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 3.311611435151460063*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        3.311611435151460063 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.103870478383820021*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.157695782626260003 + 0*_Complex_I,
        0 + 0.315391565252520006*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.946174695757560018 + 0*_Complex_I,
        0 + 0.315391565252520006*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.892349391515120036*_Complex_I,
        0 + 0*_Complex_I,
        0.157695782626260003 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.946174695757560018 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.273137107648019767 + 0*_Complex_I,
        0 - 0.546274215296039535*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.638822645888118605 + 0*_Complex_I,
        0 - 0.546274215296039535*_Complex_I,
        0 + 0*_Complex_I,
        0 + 3.277645291776237211*_Complex_I,
        0 + 0*_Complex_I,
       -0.273137107648019767 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.638822645888118605 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.819411322944059302 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.819411322944059302*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.819411322944059302 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.092548430592079070 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.819411322944059302*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.092548430592079070*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.057855469152043038 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.057855469152043038*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.057855469152043038 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.410473958869390717 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.057855469152043038*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.410473958869390717*_Complex_I,
        0 + 0*_Complex_I,
        0.211571093830408607 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.423142187660817215 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.692568750643268860 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.211571093830408607 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.692568750643268860 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.564189583547756286 + 0*_Complex_I,
       -0.211571093830408607 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.423142187660817215 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.692568750643268860 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.211571093830408607 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.692568750643268860 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.564189583547756286 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.057855469152043038 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.057855469152043038*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.057855469152043038 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.410473958869390717 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.057855469152043038*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.410473958869390717*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.819411322944059302 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.819411322944059302*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.819411322944059302 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.092548430592079070 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.819411322944059302*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.092548430592079070*_Complex_I,
        0 + 0*_Complex_I,
       -0.273137107648019767 + 0*_Complex_I,
        0 - 0.546274215296039535*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.638822645888118605 + 0*_Complex_I,
        0 - 0.546274215296039535*_Complex_I,
        0 + 0*_Complex_I,
        0 + 3.277645291776237211*_Complex_I,
        0 + 0*_Complex_I,
        0.273137107648019767 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.638822645888118605 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.157695782626260003 + 0*_Complex_I,
        0 + 0.315391565252520006*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.946174695757560018 + 0*_Complex_I,
        0 + 0.315391565252520006*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.892349391515120036*_Complex_I,
        0 + 0*_Complex_I,
       -0.157695782626260003 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.946174695757560018 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.103870478383820021 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 3.311611435151460063*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        3.311611435151460063 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.103870478383820021*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.417223823632784089 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.251671470898352269*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.251671470898352269 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.417223823632784089*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.417223823632784089 + 0*_Complex_I,
        0 + 1.668895294531136358*_Complex_I,
        0 + 0*_Complex_I,
       -2.503342941796704538 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.668895294531136358*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.417223823632784089 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.442532692444982632 + 0*_Complex_I,
        0 - 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
       -2.655196154669895796 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.442532692444982632 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.147510897481660877 + 0*_Complex_I,
        0 - 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
       -0.885065384889965265 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.147510897481660877 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.180087179853287020 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 3.540261539559861062*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.540261539559861062 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.180087179853287020*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.590043589926643510 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.770130769779930531 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.295021794963321755 + 0*_Complex_I,
        0 + 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.770130769779930531 + 0*_Complex_I,
        0 + 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
        0 - 3.540261539559861062*_Complex_I,
        0 + 0*_Complex_I,
        0.295021794963321755 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.770130769779930531 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.193137101011594790 + 0*_Complex_I,
        0 + 0.386274202023189580*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.158822606069568741 + 0*_Complex_I,
        0 + 0.386274202023189580*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.317645212139137482*_Complex_I,
        0 + 0*_Complex_I,
        0.193137101011594790 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.158822606069568741 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.158822606069568741 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.158822606069568741*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.158822606069568741 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.545096808092758321 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.158822606069568741*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.545096808092758321*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.946174695757560018 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.946174695757560018*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.946174695757560018 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.261566261010080024 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.946174695757560018*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.261566261010080024*_Complex_I,
        0 + 0*_Complex_I,
        0.236543673939390004 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.473087347878780009 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.892349391515120036 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.236543673939390004 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.892349391515120036 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.630783130505040012 + 0*_Complex_I,
        0.236543673939390004 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.473087347878780009 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.892349391515120036 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.236543673939390004 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.892349391515120036 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.630783130505040012 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.946174695757560018 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.946174695757560018*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.946174695757560018 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.261566261010080024 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.946174695757560018*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.261566261010080024*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.158822606069568741 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.158822606069568741*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.158822606069568741 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.545096808092758321 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.158822606069568741*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.545096808092758321*_Complex_I,
        0 + 0*_Complex_I,
       -0.193137101011594790 + 0*_Complex_I,
        0 - 0.386274202023189580*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.158822606069568741 + 0*_Complex_I,
        0 - 0.386274202023189580*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.317645212139137482*_Complex_I,
        0 + 0*_Complex_I,
        0.193137101011594790 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.158822606069568741 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.295021794963321755 + 0*_Complex_I,
        0 - 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.770130769779930531 + 0*_Complex_I,
        0 - 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
        0 + 3.540261539559861062*_Complex_I,
        0 + 0*_Complex_I,
        0.295021794963321755 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.770130769779930531 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.590043589926643510 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.770130769779930531 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.180087179853287020 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 3.540261539559861062*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        3.540261539559861062 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.180087179853287020*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.147510897481660877 + 0*_Complex_I,
        0 + 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
       -0.885065384889965265 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.147510897481660877 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.442532692444982632 + 0*_Complex_I,
        0 + 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
       -2.655196154669895796 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.442532692444982632 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.442532692444982632 + 0*_Complex_I,
        0 + 2.212663462224913163*_Complex_I,
        0 + 0*_Complex_I,
        4.425326924449826327 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 4.425326924449826327*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.212663462224913163 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.442532692444982632*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.442532692444982632 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.655196154669895796 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.442532692444982632 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.327598077334947898 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 5.310392309339791593*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        7.965588464009687389 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 5.310392309339791593*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.327598077334947898 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.147510897481660877 + 0*_Complex_I,
        0 + 0.442532692444982632*_Complex_I,
        0 + 0*_Complex_I,
        0.295021794963321755 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.180087179853287020 + 0*_Complex_I,
        0 + 0.295021794963321755*_Complex_I,
        0 + 0*_Complex_I,
        0 - 3.540261539559861062*_Complex_I,
        0 + 0*_Complex_I,
        0.442532692444982632 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.540261539559861062 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.147510897481660877*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.180087179853287020*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.295021794963321755 + 0*_Complex_I,
        0 - 0.885065384889965265*_Complex_I,
        0 + 0*_Complex_I,
       -0.590043589926643510 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.360174359706574041 + 0*_Complex_I,
        0 - 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
        0 + 7.080523079119722124*_Complex_I,
        0 + 0*_Complex_I,
       -0.885065384889965265 + 0*_Complex_I,
        0 + 0*_Complex_I,
        7.080523079119722124 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.295021794963321755*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.360174359706574041*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.885065384889965265 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.770130769779930531 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 - 3.540261539559861062*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.885065384889965265 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.770130769779930531 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.351959707081163531 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.703919414162327062*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.703919414162327062 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.703919414162327062*_Complex_I,
        0 + 0*_Complex_I,
        0 + 5.407838828324654124*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.351959707081163531 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.703919414162327062 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.193137101011594790 + 0*_Complex_I,
        0 - 0.193137101011594790*_Complex_I,
        0 + 0*_Complex_I,
        0.386274202023189580 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.317645212139137482 + 0*_Complex_I,
        0 - 0.386274202023189580*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.317645212139137482*_Complex_I,
        0 + 0*_Complex_I,
        0.193137101011594790 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.317645212139137482 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.545096808092758321 + 0*_Complex_I,
        0 - 0.193137101011594790*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.317645212139137482*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.545096808092758321*_Complex_I,
        0 + 0*_Complex_I,
       -0.236543673939390004 + 0*_Complex_I,
        0 + 0.236543673939390004*_Complex_I,
        0 + 0*_Complex_I,
       -0.473087347878780009 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.838524087272680054 + 0*_Complex_I,
        0 + 0.473087347878780009*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.838524087272680054*_Complex_I,
        0 + 0*_Complex_I,
       -0.236543673939390004 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.838524087272680054 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.892349391515120036 + 0*_Complex_I,
        0 + 0.236543673939390004*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.838524087272680054*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.892349391515120036*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.182718369696950022 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.365436739393900045 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.153915652525200060 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.182718369696950022 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.153915652525200060 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.630783130505040012 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.182718369696950022 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.365436739393900045 + 0*_Complex_I,
        0 + 0*_Complex_I,
        3.153915652525200060 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.182718369696950022 + 0*_Complex_I,
        0 + 0*_Complex_I,
        3.153915652525200060 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.630783130505040012 + 0*_Complex_I,
       -0.236543673939390004 + 0*_Complex_I,
        0 - 0.236543673939390004*_Complex_I,
        0 + 0*_Complex_I,
       -0.473087347878780009 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.838524087272680054 + 0*_Complex_I,
        0 - 0.473087347878780009*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.838524087272680054*_Complex_I,
        0 + 0*_Complex_I,
       -0.236543673939390004 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.838524087272680054 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.892349391515120036 + 0*_Complex_I,
        0 - 0.236543673939390004*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.838524087272680054*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.892349391515120036*_Complex_I,
        0 + 0*_Complex_I,
        0.193137101011594790 + 0*_Complex_I,
        0 + 0.193137101011594790*_Complex_I,
        0 + 0*_Complex_I,
        0.386274202023189580 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.317645212139137482 + 0*_Complex_I,
        0 + 0.386274202023189580*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.317645212139137482*_Complex_I,
        0 + 0*_Complex_I,
        0.193137101011594790 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.317645212139137482 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.545096808092758321 + 0*_Complex_I,
        0 + 0.193137101011594790*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.317645212139137482*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.545096808092758321*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.351959707081163531 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.703919414162327062*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.703919414162327062 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.703919414162327062*_Complex_I,
        0 + 0*_Complex_I,
        0 + 5.407838828324654124*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.351959707081163531 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.703919414162327062 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.885065384889965265 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.770130769779930531 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 - 3.540261539559861062*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.885065384889965265 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.770130769779930531 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.295021794963321755 + 0*_Complex_I,
        0 + 0.885065384889965265*_Complex_I,
        0 + 0*_Complex_I,
       -0.590043589926643510 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.360174359706574041 + 0*_Complex_I,
        0 + 0.590043589926643510*_Complex_I,
        0 + 0*_Complex_I,
        0 - 7.080523079119722124*_Complex_I,
        0 + 0*_Complex_I,
       -0.885065384889965265 + 0*_Complex_I,
        0 + 0*_Complex_I,
        7.080523079119722124 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.295021794963321755*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.360174359706574041*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.147510897481660877 + 0*_Complex_I,
        0 - 0.442532692444982632*_Complex_I,
        0 + 0*_Complex_I,
        0.295021794963321755 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.180087179853287020 + 0*_Complex_I,
        0 - 0.295021794963321755*_Complex_I,
        0 + 0*_Complex_I,
        0 + 3.540261539559861062*_Complex_I,
        0 + 0*_Complex_I,
        0.442532692444982632 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.540261539559861062 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0.147510897481660877*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.180087179853287020*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.327598077334947898 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 5.310392309339791593*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -7.965588464009687389 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 5.310392309339791593*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.327598077334947898 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.442532692444982632 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.655196154669895796 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.770130769779930531*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.442532692444982632 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.442532692444982632 + 0*_Complex_I,
        0 - 2.212663462224913163*_Complex_I,
        0 + 0*_Complex_I,
        4.425326924449826327 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 4.425326924449826327*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.212663462224913163 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.442532692444982632*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.464132203440858160 + 0*_Complex_I,
        0 - 2.320661017204290803*_Complex_I,
        0 + 0*_Complex_I,
       -4.641322034408581606 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 4.641322034408581606*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.320661017204290803 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.464132203440858160*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.139941124721293271 + 0*_Complex_I,
        0 - 0.699705623606466358*_Complex_I,
        0 + 0*_Complex_I,
       -1.399411247212932717 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.399411247212932717*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.699705623606466358 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.139941124721293271*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.399411247212932717 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 5.597644988851730871*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -8.396467483277596306 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 5.597644988851730871*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.399411247212932717 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.625835735449176134 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.503342941796704538*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.755014412695056807 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.503342941796704538*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.625835735449176134 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.312917867724588067 + 0*_Complex_I,
        0 + 0.938753603173764201*_Complex_I,
        0 + 0*_Complex_I,
        0.625835735449176134 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.503342941796704538 + 0*_Complex_I,
        0 + 0.625835735449176134*_Complex_I,
        0 + 0*_Complex_I,
        0 - 7.510028825390113615*_Complex_I,
        0 + 0*_Complex_I,
        0.938753603173764201 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -7.510028825390113615 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.312917867724588067*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.503342941796704538*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.180663215165034628 + 0*_Complex_I,
        0 + 0.541989645495103885*_Complex_I,
        0 + 0*_Complex_I,
        0.361326430330069256 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.445305721320277027 + 0*_Complex_I,
        0 + 0.361326430330069256*_Complex_I,
        0 + 0*_Complex_I,
        0 - 4.335917163960831083*_Complex_I,
        0 + 0*_Complex_I,
        0.541989645495103885 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -4.335917163960831083 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.180663215165034628*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.445305721320277027*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.445305721320277027 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.890611442640554055*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.890611442640554055 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.890611442640554055*_Complex_I,
        0 + 0*_Complex_I,
        0 - 5.781222885281108110*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.445305721320277027 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.890611442640554055 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.021985476433282363 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.043970952866564726*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.043970952866564726 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.043970952866564726*_Complex_I,
        0 + 0*_Complex_I,
        0 - 4.087941905733129453*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.021985476433282363 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.043970952866564726 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.255496369108320590 + 0*_Complex_I,
        0 - 0.255496369108320590*_Complex_I,
        0 + 0*_Complex_I,
        0.510992738216641181 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.065956429299847090 + 0*_Complex_I,
        0 - 0.510992738216641181*_Complex_I,
        0 + 0*_Complex_I,
        0 + 3.065956429299847090*_Complex_I,
        0 + 0*_Complex_I,
        0.255496369108320590 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.065956429299847090 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.043970952866564726 + 0*_Complex_I,
        0 - 0.255496369108320590*_Complex_I,
        0 + 0*_Complex_I,
        0 + 3.065956429299847090*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.043970952866564726*_Complex_I,
        0 + 0*_Complex_I,
        0.215933843419584674 + 0*_Complex_I,
        0 - 0.215933843419584674*_Complex_I,
        0 + 0*_Complex_I,
        0.431867686839169349 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.591206121035016094 + 0*_Complex_I,
        0 - 0.431867686839169349*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.591206121035016094*_Complex_I,
        0 + 0*_Complex_I,
        0.215933843419584674 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.591206121035016094 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.727470747356677396 + 0*_Complex_I,
        0 - 0.215933843419584674*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.591206121035016094*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.727470747356677396*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.295603060517508047 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.591206121035016094 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.454941494713354792 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.295603060517508047 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.454941494713354792 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.690988298942670958 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.295603060517508047 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.591206121035016094 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.454941494713354792 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.295603060517508047 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.454941494713354792 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.690988298942670958 + 0*_Complex_I,
       -0.215933843419584674 + 0*_Complex_I,
        0 - 0.215933843419584674*_Complex_I,
        0 + 0*_Complex_I,
       -0.431867686839169349 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.591206121035016094 + 0*_Complex_I,
        0 - 0.431867686839169349*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.591206121035016094*_Complex_I,
        0 + 0*_Complex_I,
       -0.215933843419584674 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.591206121035016094 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.727470747356677396 + 0*_Complex_I,
        0 - 0.215933843419584674*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.591206121035016094*_Complex_I,
        0 + 0*_Complex_I,
        0 - 1.727470747356677396*_Complex_I,
        0 + 0*_Complex_I,
       -0.255496369108320590 + 0*_Complex_I,
        0 - 0.255496369108320590*_Complex_I,
        0 + 0*_Complex_I,
       -0.510992738216641181 + 0*_Complex_I,
        0 + 0*_Complex_I,
        3.065956429299847090 + 0*_Complex_I,
        0 - 0.510992738216641181*_Complex_I,
        0 + 0*_Complex_I,
        0 + 3.065956429299847090*_Complex_I,
        0 + 0*_Complex_I,
       -0.255496369108320590 + 0*_Complex_I,
        0 + 0*_Complex_I,
        3.065956429299847090 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.043970952866564726 + 0*_Complex_I,
        0 - 0.255496369108320590*_Complex_I,
        0 + 0*_Complex_I,
        0 + 3.065956429299847090*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.043970952866564726*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.021985476433282363 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.043970952866564726*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.043970952866564726 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.043970952866564726*_Complex_I,
        0 + 0*_Complex_I,
        0 + 4.087941905733129453*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.021985476433282363 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.043970952866564726 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.445305721320277027 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.890611442640554055*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        2.890611442640554055 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.890611442640554055*_Complex_I,
        0 + 0*_Complex_I,
        0 + 5.781222885281108110*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.445305721320277027 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.890611442640554055 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.180663215165034628 + 0*_Complex_I,
        0 + 0.541989645495103885*_Complex_I,
        0 + 0*_Complex_I,
       -0.361326430330069256 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -1.445305721320277027 + 0*_Complex_I,
        0 + 0.361326430330069256*_Complex_I,
        0 + 0*_Complex_I,
        0 - 4.335917163960831083*_Complex_I,
        0 + 0*_Complex_I,
       -0.541989645495103885 + 0*_Complex_I,
        0 + 0*_Complex_I,
        4.335917163960831083 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.180663215165034628*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.445305721320277027*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.312917867724588067 + 0*_Complex_I,
        0 + 0.938753603173764201*_Complex_I,
        0 + 0*_Complex_I,
       -0.625835735449176134 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.503342941796704538 + 0*_Complex_I,
        0 + 0.625835735449176134*_Complex_I,
        0 + 0*_Complex_I,
        0 - 7.510028825390113615*_Complex_I,
        0 + 0*_Complex_I,
       -0.938753603173764201 + 0*_Complex_I,
        0 + 0*_Complex_I,
        7.510028825390113615 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.312917867724588067*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.503342941796704538*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.625835735449176134 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 2.503342941796704538*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -3.755014412695056807 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 2.503342941796704538*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0.625835735449176134 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.399411247212932717 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 5.597644988851730871*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -8.396467483277596306 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 5.597644988851730871*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        1.399411247212932717 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.139941124721293271 + 0*_Complex_I,
        0 - 0.699705623606466358*_Complex_I,
        0 + 0*_Complex_I,
        1.399411247212932717 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 1.399411247212932717*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.699705623606466358 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.139941124721293271*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -0.464132203440858160 + 0*_Complex_I,
        0 - 2.320661017204290803*_Complex_I,
        0 + 0*_Complex_I,
        4.641322034408581606 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 4.641322034408581606*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
       -2.320661017204290803 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 - 0.464132203440858160*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0*_Complex_I,
        0 + 0};

static inline int _len_spinor(int l, int kappa)
{
        if (0 == kappa) {
                return 4 * l + 2;
        } else if (kappa < 0) {
                return 2 * l + 2;
        } else {
                return 2 * l;
        }
}

struct cart2sp_t {
        const double *cart2sph;
        const double complex *cart2j_lt_l; // j < l, kappa > 0
        const double complex *cart2j_gt_l; // j > l, kappa < 0
};

// [*] = n(n+1)(n+2)(n+3)/4+(n+1)(n+2)(n+3)/6
static struct cart2sp_t g_c2s[6] = {
        {g_trans_cart2sph    , g_trans_cart2j    , g_trans_cart2j     },
        {g_trans_cart2sph+1  , g_trans_cart2j+4  , g_trans_cart2j+16  },
        {g_trans_cart2sph+10 , g_trans_cart2j+40 , g_trans_cart2j+88  },
        {g_trans_cart2sph+40 , g_trans_cart2j+160, g_trans_cart2j+280 },
        {g_trans_cart2sph+110, g_trans_cart2j+440, g_trans_cart2j+680 },
        {g_trans_cart2sph+245, g_trans_cart2j+980, g_trans_cart2j+1400}};

// transform integrals from cartesian to spheric
static double *a_bra_cart2spheric(double *gsph, int nket, double *gcart, int l)
{
        const double D0 = 0;
        const double D1 = 1;
        const char TRANS_T = 'T';
        const char TRANS_N = 'N';
        const int nf = CINTlen_cart(l);
        const int nd = l * 2 + 1;
        dgemm_(&TRANS_T, &TRANS_N, &nd, &nket, &nf,
               &D1, g_c2s[l].cart2sph, &nf, gcart, &nf,
               &D0, gsph, &nd);
        return gsph;
}

static double *a_ket_cart2spheric(double *gsph, int nbra, double *gcart, int l)
{
        const double D0 = 0;
        const double D1 = 1;
        const char TRANS_N = 'N';
        const int nf = CINTlen_cart(l);
        const int nd = l * 2 + 1;
        dgemm_(&TRANS_N, &TRANS_N, &nbra, &nd, &nf,
               &D1, gcart, &nbra, g_c2s[l].cart2sph, &nf,
               &D0, gsph, &nbra);
        return gsph;
}

// transform s function from cartesian to spheric
static double *s_bra_cart2spheric(double *gsph, int nket, double *gcart, int l)
{
        /*
        int i;
        for (i = 0; i < nket; i++) {
                *gsph = gcart[i];
        }*/
        return gcart;
}
static double *s_ket_cart2spheric(double *gsph, int nbra, double *gcart, int l)
{
        /*
        int i;
        for (i = 0; i < nbra; i++) {
                gsph[i] = gcart[i];
        }*/
        return gcart;
}

// transform p function from cartesian to spheric
static double *p_bra_cart2spheric(double *gsph, int nket, double *gcart, int l)
{
        /*
        double *pgcart = gcart;
        int i;
        for (i = 0; i < nket; i++) {
                gsph[0] = gcart[0];
                gsph[1] = gcart[1];
                gsph[2] = gcart[2];
                gsph += 3;
                gcart += 3;
        }
        return pgcart;*/
        return gcart;
}
static double *p_ket_cart2spheric(double *gsph, int nbra, double *gcart, int l)
{
        /*
        int i;
        for (i = 0; i < nbra; i++) {
                gsph[0*nbra+i] = gcart[0*nbra+i];
                gsph[1*nbra+i] = gcart[1*nbra+i];
                gsph[2*nbra+i] = gcart[2*nbra+i];
        }*/
        return gcart;
}

// transform d function from cartesian to spheric
static double *d_bra_cart2spheric(double *gsph, int nket, double *gcart, int l)
{
        const double *coeff_c2s = g_c2s[2].cart2sph;
        double *const pgsph = gsph;
        int i;
        for (i = 0; i < nket; i++) {
                gsph[0] = coeff_c2s[ 1] * gcart[1];
                gsph[1] = coeff_c2s[10] * gcart[4];
                gsph[2] = coeff_c2s[12] * gcart[0]
                        + coeff_c2s[15] * gcart[3]
                        + coeff_c2s[17] * gcart[5];
                gsph[3] = coeff_c2s[20] * gcart[2];
                gsph[4] = coeff_c2s[24] * gcart[0]
                        + coeff_c2s[27] * gcart[3];
                gsph += 5;
                gcart += 6;
        }
        return pgsph;
}
static double *d_ket_cart2spheric(double *gsph, int nbra, double *gcart, int l)
{
        const double *coeff_c2s = g_c2s[2].cart2sph;
        double *const pgsph = gsph;
        int i;
        for (i = 0; i < nbra; i++) {
                gsph[0*nbra+i] = coeff_c2s[ 1] * gcart[1*nbra+i];
                gsph[1*nbra+i] = coeff_c2s[10] * gcart[4*nbra+i];
                gsph[2*nbra+i] = coeff_c2s[12] * gcart[0*nbra+i]
                               + coeff_c2s[15] * gcart[3*nbra+i]
                               + coeff_c2s[17] * gcart[5*nbra+i];
                gsph[3*nbra+i] = coeff_c2s[20] * gcart[2*nbra+i];
                gsph[4*nbra+i] = coeff_c2s[24] * gcart[0*nbra+i]
                               + coeff_c2s[27] * gcart[3*nbra+i];
        }
        return pgsph;
}

// transform f function from cartesian to spheric
static double *f_bra_cart2spheric(double *gsph, int nket, double *gcart, int l)
{
        const double *coeff_c2s = g_c2s[3].cart2sph;
        double *const pgsph = gsph;
        int i;
        for (i = 0; i < nket; i++) {
                gsph[0] = coeff_c2s[ 1] * gcart[1]
                        + coeff_c2s[ 6] * gcart[6];
                gsph[1] = coeff_c2s[14] * gcart[4];
                gsph[2] = coeff_c2s[21] * gcart[1]
                        + coeff_c2s[26] * gcart[6]
                        + coeff_c2s[28] * gcart[8];
                gsph[3] = coeff_c2s[32] * gcart[2]
                        + coeff_c2s[37] * gcart[7]
                        + coeff_c2s[39] * gcart[9];
                gsph[4] = coeff_c2s[40] * gcart[0]
                        + coeff_c2s[43] * gcart[3]
                        + coeff_c2s[45] * gcart[5];
                gsph[5] = coeff_c2s[52] * gcart[2]
                        + coeff_c2s[57] * gcart[7];
                gsph[6] = coeff_c2s[60] * gcart[0]
                        + coeff_c2s[63] * gcart[3];
                gsph += 7;
                gcart += 10;
        }
        return pgsph;
}
static double *f_ket_cart2spheric(double *gsph, int nbra, double *gcart, int l)
{
        const double *coeff_c2s = g_c2s[3].cart2sph;
        double *const pgsph = gsph;
        int i;
        for (i = 0; i < nbra; i++) {
                gsph[0*nbra+i] = coeff_c2s[ 1] * gcart[1*nbra+i]
                               + coeff_c2s[ 6] * gcart[6*nbra+i];
                gsph[1*nbra+i] = coeff_c2s[14] * gcart[4*nbra+i];
                gsph[2*nbra+i] = coeff_c2s[21] * gcart[1*nbra+i]
                               + coeff_c2s[26] * gcart[6*nbra+i]
                               + coeff_c2s[28] * gcart[8*nbra+i];
                gsph[3*nbra+i] = coeff_c2s[32] * gcart[2*nbra+i]
                               + coeff_c2s[37] * gcart[7*nbra+i]
                               + coeff_c2s[39] * gcart[9*nbra+i];
                gsph[4*nbra+i] = coeff_c2s[40] * gcart[0*nbra+i]
                               + coeff_c2s[43] * gcart[3*nbra+i]
                               + coeff_c2s[45] * gcart[5*nbra+i];
                gsph[5*nbra+i] = coeff_c2s[52] * gcart[2*nbra+i]
                               + coeff_c2s[57] * gcart[7*nbra+i];
                gsph[6*nbra+i] = coeff_c2s[60] * gcart[0*nbra+i]
                               + coeff_c2s[63] * gcart[3*nbra+i];
        }
        return pgsph;
}

// transform g function from cartesian to spheric
static double *g_bra_cart2spheric(double *gsph, int nket, double *gcart, int l)
{
        const double *coeff_c2s = g_c2s[4].cart2sph;
        double *const pgsph = gsph;
        int i;
        for (i = 0; i < nket; i++) {
                gsph[0] = coeff_c2s[  1] * gcart[ 1]
                        + coeff_c2s[  6] * gcart[ 6];
                gsph[1] = coeff_c2s[ 19] * gcart[ 4]
                        + coeff_c2s[ 26] * gcart[11];
                gsph[2] = coeff_c2s[ 31] * gcart[ 1]
                        + coeff_c2s[ 36] * gcart[ 6]
                        + coeff_c2s[ 38] * gcart[ 8];
                gsph[3] = coeff_c2s[ 49] * gcart[ 4]
                        + coeff_c2s[ 56] * gcart[11]
                        + coeff_c2s[ 58] * gcart[13];
                gsph[4] = coeff_c2s[ 60] * gcart[ 0]
                        + coeff_c2s[ 63] * gcart[ 3]
                        + coeff_c2s[ 65] * gcart[ 5]
                        + coeff_c2s[ 70] * gcart[10]
                        + coeff_c2s[ 72] * gcart[12]
                        + coeff_c2s[ 74] * gcart[14];
                gsph[5] = coeff_c2s[ 77] * gcart[ 2]
                        + coeff_c2s[ 82] * gcart[ 7]
                        + coeff_c2s[ 84] * gcart[ 9];
                gsph[6] = coeff_c2s[ 90] * gcart[ 0]
                        + coeff_c2s[ 95] * gcart[ 5]
                        + coeff_c2s[100] * gcart[10]
                        + coeff_c2s[102] * gcart[12];
                gsph[7] = coeff_c2s[107] * gcart[ 2]
                        + coeff_c2s[112] * gcart[ 7];
                gsph[8] = coeff_c2s[120] * gcart[ 0]
                        + coeff_c2s[123] * gcart[ 3]
                        + coeff_c2s[130] * gcart[10];
                gsph += 9;
                gcart += 15;
        }
        return pgsph;
}
static double *g_ket_cart2spheric(double *gsph, int nbra, double *gcart, int l)
{
        const double *coeff_c2s = g_c2s[4].cart2sph;
        double *const pgsph = gsph;
        int i;
        for (i = 0; i < nbra; i++) {
                gsph[0*nbra+i] = coeff_c2s[  1] * gcart[ 1*nbra+i]
                               + coeff_c2s[  6] * gcart[ 6*nbra+i];
                gsph[1*nbra+i] = coeff_c2s[ 19] * gcart[ 4*nbra+i]
                               + coeff_c2s[ 26] * gcart[11*nbra+i];
                gsph[2*nbra+i] = coeff_c2s[ 31] * gcart[ 1*nbra+i]
                               + coeff_c2s[ 36] * gcart[ 6*nbra+i]
                               + coeff_c2s[ 38] * gcart[ 8*nbra+i];
                gsph[3*nbra+i] = coeff_c2s[ 49] * gcart[ 4*nbra+i]
                               + coeff_c2s[ 56] * gcart[11*nbra+i]
                               + coeff_c2s[ 58] * gcart[13*nbra+i];
                gsph[4*nbra+i] = coeff_c2s[ 60] * gcart[ 0*nbra+i]
                               + coeff_c2s[ 63] * gcart[ 3*nbra+i]
                               + coeff_c2s[ 65] * gcart[ 5*nbra+i]
                               + coeff_c2s[ 70] * gcart[10*nbra+i]
                               + coeff_c2s[ 72] * gcart[12*nbra+i]
                               + coeff_c2s[ 74] * gcart[14*nbra+i];
                gsph[5*nbra+i] = coeff_c2s[ 77] * gcart[ 2*nbra+i]
                               + coeff_c2s[ 82] * gcart[ 7*nbra+i]
                               + coeff_c2s[ 84] * gcart[ 9*nbra+i];
                gsph[6*nbra+i] = coeff_c2s[ 90] * gcart[ 0*nbra+i]
                               + coeff_c2s[ 95] * gcart[ 5*nbra+i]
                               + coeff_c2s[100] * gcart[10*nbra+i]
                               + coeff_c2s[102] * gcart[12*nbra+i];
                gsph[7*nbra+i] = coeff_c2s[107] * gcart[ 2*nbra+i]
                               + coeff_c2s[112] * gcart[ 7*nbra+i];
                gsph[8*nbra+i] = coeff_c2s[120] * gcart[ 0*nbra+i]
                               + coeff_c2s[123] * gcart[ 3*nbra+i]
                               + coeff_c2s[130] * gcart[10*nbra+i];
        }
        return pgsph;
}

/*
 * return the address of gemm results, for s,p function, results ==
 * input, so return the input address optimize
 */
static double *(*f_bra_sph[6])() = {
        s_bra_cart2spheric,
        p_bra_cart2spheric,
        d_bra_cart2spheric,
        f_bra_cart2spheric,
        g_bra_cart2spheric,
        a_bra_cart2spheric,
};

static double *(*f_ket_sph[6])() = {
        s_ket_cart2spheric,
        p_ket_cart2spheric,
        d_ket_cart2spheric,
        f_ket_cart2spheric,
        g_ket_cart2spheric,
        a_ket_cart2spheric,
};


// transform spin free integrals from cartesian to spinor
static void a_bra_cart2spinor_sf(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        const double complex Z0 = 0;
        const double complex Z1 = 1;
        const char TRANS_C = 'C';
        const char TRANS_N = 'N';
        const int nf = CINTlen_cart(l);
        const int nf2 = nf * 2;
        const int nd = _len_spinor(l, kappa);
        const double complex *coeff_c2s;

        if (kappa < 0) { // j = l + 1/2
                coeff_c2s = g_c2s[l].cart2j_gt_l;
        } else {
                coeff_c2s = g_c2s[l].cart2j_lt_l;
        }
        zgemm_(&TRANS_C, &TRANS_N, &nd, &nket, &nf,
               &Z1, coeff_c2s, &nf2, gcart, &nf, &Z0, gsp, &nd);
        zgemm_(&TRANS_C, &TRANS_N, &nd, &nket, &nf,
               &Z1, coeff_c2s+nf, &nf2, gcart, &nf,
               &Z0, gsp+nd*nket, &nd);
}
static void a_bra_cart2spinor_e1sf(double complex *gsp, int nket,
                                   double *gcart, int l, int kappa)
{
        const int nf = CINTlen_cart(l);
        double complex *tmp1 = malloc(sizeof(double complex)*nf*nket);

        CINTdcmplx_re(nf*nket, tmp1, gcart);
        a_bra_cart2spinor_sf(gsp, nket, tmp1, l, kappa);
        free(tmp1);
}

static void a_bra_cart2spinor_si(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        const double complex Z0 = 0;
        const double complex Z1 = 1;
        const char TRANS_C = 'C';
        const char TRANS_N = 'N';
        const int nf = CINTlen_cart(l);
        const int nf2 = nf * 2;
        const int nd = _len_spinor(l, kappa);
        const double complex *coeff_c2s;

        if (kappa < 0) { // j = l + 1/2
                coeff_c2s = g_c2s[l].cart2j_gt_l;
        } else {
                coeff_c2s = g_c2s[l].cart2j_lt_l;
        }
        zgemm_(&TRANS_C, &TRANS_N, &nd, &nket, &nf,
               &Z1, coeff_c2s, &nf2, gcart, &nf, &Z0, gsp, &nd);
        zgemm_(&TRANS_C, &TRANS_N, &nd, &nket, &nf,
               &Z1, coeff_c2s+nf, &nf2,
               gcart+nf*nket, &nf, &Z1, gsp, &nd);
}

static void a_ket_cart2spinor(double complex *gsp, int nbra,
                              double complex *gcart, int l, int kappa)
{
        const double complex Z0 = 0;
        const double complex Z1 = 1;
        const char TRANS_N = 'N';
        const int nf = CINTlen_cart(l);
        const int nf2 = nf * 2;
        const int nd = _len_spinor(l, kappa);
        const double complex *coeff_c2s;

        if (kappa < 0) { // j = l + 1/2
                coeff_c2s = g_c2s[l].cart2j_gt_l;
        } else {
                coeff_c2s = g_c2s[l].cart2j_lt_l;
        }
        zgemm_(&TRANS_N, &TRANS_N, &nbra, &nd, &nf2,
               &Z1, gcart, &nbra, coeff_c2s, &nf2, &Z0, gsp, &nbra);
}
// with phase "i"
static void a_iket_cart2spinor(double complex *gsp, int nbra,
                               double complex *gcart, int l, int kappa)
{
        const double complex Z0 = 0;
        const double complex ZI = 0 + 1 * _Complex_I;
        const char TRANS_N = 'N';
        const int nf = CINTlen_cart(l);
        const int nf2 = nf * 2;
        const int nd = _len_spinor(l, kappa);
        const double complex *coeff_c2s;

        if (kappa < 0) { // j = l + 1/2
                coeff_c2s = g_c2s[l].cart2j_gt_l;
        } else {
                coeff_c2s = g_c2s[l].cart2j_lt_l;
        }
        zgemm_(&TRANS_N, &TRANS_N, &nbra, &nd, &nf2,
               &ZI, gcart, &nbra, coeff_c2s, &nf2, &Z0, gsp, &nbra);
}

static void s_bra_cart2spinor_sf(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        //const double *coeff_c2s = g_c2s[0].cart2j_lt_l;
        double complex *gsp1 = gsp + nket * 2;
        int i;
        for (i = 0; i < nket; i++) {
                gsp [i*2+0] = 0;
                gsp [i*2+1] = gcart[i];
                gsp1[i*2+0] = gcart[i];
                gsp1[i*2+1] = 0;
        }
}
static void s_bra_cart2spinor_e1sf(double complex *gsp, int nket,
                                   double *gcart, int l, int kappa)
{
        //const double *coeff_c2s = g_c2s[0].cart2j_lt_l;;
        double complex *gsp1 = gsp + nket * 2;
        int i;
        for (i = 0; i < nket; i++) {
                gsp [i*2+0] = 0;
                gsp [i*2+1] = gcart[i];
                gsp1[i*2+0] = gcart[i];
                gsp1[i*2+1] = 0;
        }
}
static void s_bra_cart2spinor_si(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        //const double *coeff_c2s = g_c2s[0].cart2j_lt_l;;
        double complex *gcart1 = gcart + nket;
        int i;
        for (i = 0; i < nket; i++) {
                gsp[i*2+0] = gcart1[i];
                gsp[i*2+1] = gcart [i];
        }
}
static void s_ket_cart2spinor(double complex *gsp, int nbra,
                              double complex *gcart, int l, int kappa)
{
        //const double *coeff_c2s = g_c2s[0].cart2j_lt_l;;
        double complex *gsp1 = gsp + nbra;
        double complex *gcart1 = gcart + nbra;
        int i;
        for (i = 0; i < nbra; i++) {
                gsp [i] = gcart1[i];
                gsp1[i] = gcart [i];
        }
}
static void s_iket_cart2spinor(double complex *gsp, int nbra,
                               double complex *gcart, int l, int kappa)
{
        //const double *coeff_c2s = g_c2s[0].cart2j_lt_l;;
        double complex *gsp1 = gsp + nbra;
        double complex *gcart1 = gcart + nbra;
        int i;
        for (i = 0; i < nbra; i++) {
                gsp [i] = gcart1[i] * _Complex_I;
                gsp1[i] = gcart [i] * _Complex_I;
        }
}

static void p_bra_cart2spinor_sf(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        double complex *gsp1 = gsp + nket * nd;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[1].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        // spin-up part
                        gsp [i*nd+0] = creal(coeff_c2s[0])*gcart[3*i+0]
                                     - cimag(coeff_c2s[1])*gcart[3*i+1]*_Complex_I;
                        gsp [i*nd+1] = creal(coeff_c2s[8])*gcart[3*i+2];
                        // spin-down part
                        gsp1[i*nd+0] = creal(coeff_c2s[5])*gcart[3*i+2];
                        gsp1[i*nd+1] = creal(coeff_c2s[9])*gcart[3*i+0]
                                     - cimag(coeff_c2s[10])*gcart[3*i+1]*_Complex_I;
                }
                gsp  += 2;
                gsp1 += 2;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[1].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = 0;
                        gsp [i*nd+1] = creal(coeff_c2s[6])*gcart[3*i+0]
                                     - cimag(coeff_c2s[7])*gcart[3*i+1]*_Complex_I;
                        gsp [i*nd+2] = creal(coeff_c2s[14])*gcart[3*i+2];
                        gsp [i*nd+3] = creal(coeff_c2s[18])*gcart[3*i+0]
                                     - cimag(coeff_c2s[19])*gcart[3*i+1]*_Complex_I;
                        gsp1[i*nd+0] = creal(coeff_c2s[3])*gcart[3*i+0]
                                     - cimag(coeff_c2s[4])*gcart[3*i+1]*_Complex_I;
                        gsp1[i*nd+1] = creal(coeff_c2s[11])*gcart[3*i+2];
                        gsp1[i*nd+2] = creal(coeff_c2s[15])*gcart[3*i+0]
                                     - cimag(coeff_c2s[16])*gcart[3*i+1]*_Complex_I;
                        gsp1[i*nd+3] = 0;
                }
        }
}
static void p_bra_cart2spinor_e1sf(double complex *gsp, int nket,
                                   double *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        double complex *gsp1 = gsp + nket * nd;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[1].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        // spin-up part
                        gsp [i*nd+0] = creal(coeff_c2s[0])*gcart[3*i+0]
                                     - cimag(coeff_c2s[1])*gcart[3*i+1]*_Complex_I;
                        gsp [i*nd+1] = creal(coeff_c2s[8])*gcart[3*i+2];
                        // spin-down part
                        gsp1[i*nd+0] = creal(coeff_c2s[5])*gcart[3*i+2];
                        gsp1[i*nd+1] = creal(coeff_c2s[9])*gcart[3*i+0]
                                     - cimag(coeff_c2s[10])*gcart[3*i+1]*_Complex_I;
                }
                gsp  += 2;
                gsp1 += 2;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[1].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = 0;
                        gsp [i*nd+1] = creal(coeff_c2s[6])*gcart[3*i+0]
                                     - cimag(coeff_c2s[7])*gcart[3*i+1]*_Complex_I;
                        gsp [i*nd+2] = creal(coeff_c2s[14])*gcart[3*i+2];
                        gsp [i*nd+3] = creal(coeff_c2s[18])*gcart[3*i+0]
                                     - cimag(coeff_c2s[19])*gcart[3*i+1]*_Complex_I;
                        gsp1[i*nd+0] = creal(coeff_c2s[3])*gcart[3*i+0]
                                     - cimag(coeff_c2s[4])*gcart[3*i+1]*_Complex_I;
                        gsp1[i*nd+1] = creal(coeff_c2s[11])*gcart[3*i+2];
                        gsp1[i*nd+2] = creal(coeff_c2s[15])*gcart[3*i+0]
                                     - cimag(coeff_c2s[16])*gcart[3*i+1]*_Complex_I;
                        gsp1[i*nd+3] = 0;
                }
        }
}
static void p_bra_cart2spinor_si(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        double complex *gcart1 = gcart + nket * 3;
        const complex double *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[1].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        // spin-up part
                        gsp[i*nd+0] = creal(coeff_c2s[0])*gcart[3*i+0]
                                    - cimag(coeff_c2s[1])*gcart[3*i+1]*_Complex_I;
                        gsp[i*nd+1] = creal(coeff_c2s[8])*gcart[3*i+2];
                        // spin-down part
                        gsp[i*nd+0]+= creal(coeff_c2s[5])*gcart1[3*i+2];
                        gsp[i*nd+1]+= creal(coeff_c2s[9])*gcart1[3*i+0]
                                    - cimag(coeff_c2s[10])*gcart1[3*i+1]*_Complex_I;
                }
                gsp += 2;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[1].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp[i*nd+0] = 0;
                        gsp[i*nd+1] = creal(coeff_c2s[6])*gcart[3*i+0]
                                    - cimag(coeff_c2s[7])*gcart[3*i+1]*_Complex_I;
                        gsp[i*nd+2] = creal(coeff_c2s[14])*gcart[3*i+2];
                        gsp[i*nd+3] = creal(coeff_c2s[18])*gcart[3*i+0]
                                    - cimag(coeff_c2s[19])*gcart[3*i+1]*_Complex_I;

                        gsp[i*nd+0]+= creal(coeff_c2s[3])*gcart1[3*i+0]
                                    - cimag(coeff_c2s[4])*gcart1[3*i+1]*_Complex_I;
                        gsp[i*nd+1]+= creal(coeff_c2s[11])*gcart1[3*i+2];
                        gsp[i*nd+2]+= creal(coeff_c2s[15])*gcart1[3*i+0]
                                    - cimag(coeff_c2s[16])*gcart1[3*i+1]*_Complex_I;
                        gsp[i*nd+3]+= 0;
                }
        }
}
static void p_ket_cart2spinor(double complex *gsp, int nbra,
                              double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[1].cart2j_lt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[     i] = creal(coeff_c2s[0])*gcart[nbra*0+i]
                                    + cimag(coeff_c2s[1])*gcart[nbra*1+i]*_Complex_I
                                    + creal(coeff_c2s[5])*gcart[nbra*5+i];
                        gsp[nbra+i] = creal(coeff_c2s[8])*gcart[nbra*2+i]
                                    + creal(coeff_c2s[9])*gcart[nbra*3+i]
                                    + cimag(coeff_c2s[10])*gcart[nbra*4+i]*_Complex_I;
                }
                gsp += nbra * 2;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[1].cart2j_gt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[3])*gcart[nbra*3+i]
                                      + cimag(coeff_c2s[4])*gcart[nbra*4+i]*_Complex_I;
                        gsp[1*nbra+i] = creal(coeff_c2s[6])*gcart[nbra*0+i]
                                      + cimag(coeff_c2s[7])*gcart[nbra*1+i]*_Complex_I
                                      + creal(coeff_c2s[11])*gcart[nbra*5+i];
                        gsp[2*nbra+i] = creal(coeff_c2s[14])*gcart[nbra*2+i]
                                      + creal(coeff_c2s[15])*gcart[nbra*3+i]
                                      + cimag(coeff_c2s[16])*gcart[nbra*4+i]*_Complex_I;
                        gsp[3*nbra+i] = creal(coeff_c2s[18])*gcart[nbra*0+i]
                                      + cimag(coeff_c2s[19])*gcart[nbra*1+i]*_Complex_I;
                }
        }
}
static void p_iket_cart2spinor(double complex *gsp, int nbra,
                               double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[1].cart2j_lt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[     i] = creal(coeff_c2s[0])*gcart[nbra*0+i]*_Complex_I
                                    - cimag(coeff_c2s[1])*gcart[nbra*1+i]
                                    + creal(coeff_c2s[5])*gcart[nbra*5+i]*_Complex_I;
                        gsp[nbra+i] = creal(coeff_c2s[8])*gcart[nbra*2+i]*_Complex_I
                                    + creal(coeff_c2s[9])*gcart[nbra*3+i]*_Complex_I
                                    - cimag(coeff_c2s[10])*gcart[nbra*4+i];
                }
                gsp += nbra * 2;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[1].cart2j_gt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[3])*gcart[nbra*3+i]*_Complex_I
                                      - cimag(coeff_c2s[4])*gcart[nbra*4+i];
                        gsp[1*nbra+i] = creal(coeff_c2s[6])*gcart[nbra*0+i]*_Complex_I
                                      - cimag(coeff_c2s[7])*gcart[nbra*1+i]
                                      + creal(coeff_c2s[11])*gcart[nbra*5+i]*_Complex_I;
                        gsp[2*nbra+i] = creal(coeff_c2s[14])*gcart[nbra*2+i]*_Complex_I
                                      + creal(coeff_c2s[15])*gcart[nbra*3+i]*_Complex_I
                                      - cimag(coeff_c2s[16])*gcart[nbra*4+i];
                        gsp[3*nbra+i] = creal(coeff_c2s[18])*gcart[nbra*0+i]*_Complex_I
                                      - cimag(coeff_c2s[19])*gcart[nbra*1+i];
                }
        }
}

static void d_bra_cart2spinor_sf(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        double complex *gsp1 = gsp + nket * nd;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[2].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        // spin-up part
                        gsp [i*nd+0] = creal(coeff_c2s[0])*gcart[6*i+0]
                                     + creal(coeff_c2s[3])*gcart[6*i+3]
                                     - cimag(coeff_c2s[1])*gcart[6*i+1]*_Complex_I;
                        gsp [i*nd+1] = creal(coeff_c2s[14])*gcart[6*i+2]
                                     - cimag(coeff_c2s[16])*gcart[6*i+4]*_Complex_I;
                        gsp [i*nd+2] = creal(coeff_c2s[24])*gcart[6*i+0]
                                     + creal(coeff_c2s[27])*gcart[6*i+3]
                                     + creal(coeff_c2s[29])*gcart[6*i+5];
                        gsp [i*nd+3] = creal(coeff_c2s[38])*gcart[6*i+2]
                                     - cimag(coeff_c2s[40])*gcart[6*i+4]*_Complex_I;
                        // spin-down part
                        gsp1[i*nd+0] = creal(coeff_c2s[8])*gcart[6*i+2]
                                     - cimag(coeff_c2s[10])*gcart[6*i+4]*_Complex_I;
                        gsp1[i*nd+1] = creal(coeff_c2s[18])*gcart[6*i+0]
                                     + creal(coeff_c2s[21])*gcart[6*i+3]
                                     + creal(coeff_c2s[23])*gcart[6*i+5];
                        gsp1[i*nd+2] = creal(coeff_c2s[32])*gcart[6*i+2]
                                     - cimag(coeff_c2s[34])*gcart[6*i+4]*_Complex_I;
                        gsp1[i*nd+3] = creal(coeff_c2s[42])*gcart[6*i+0]
                                     + creal(coeff_c2s[45])*gcart[6*i+3]
                                     - cimag(coeff_c2s[43])*gcart[6*i+1]*_Complex_I;
                }
                gsp  += 4;
                gsp1 += 4;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[2].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = 0;
                        gsp [i*nd+1] = creal(coeff_c2s[12])*gcart[6*i+0]
                                     + creal(coeff_c2s[15])*gcart[6*i+3]
                                     - cimag(coeff_c2s[13])*gcart[6*i+1]*_Complex_I;
                        gsp [i*nd+2] = creal(coeff_c2s[26])*gcart[6*i+2]
                                     - cimag(coeff_c2s[28])*gcart[6*i+4]*_Complex_I;
                        gsp [i*nd+3] = creal(coeff_c2s[36])*gcart[6*i+0]
                                     + creal(coeff_c2s[39])*gcart[6*i+3]
                                     + creal(coeff_c2s[41])*gcart[6*i+5];
                        gsp [i*nd+4] = creal(coeff_c2s[50])*gcart[6*i+2]
                                     - cimag(coeff_c2s[52])*gcart[6*i+4]*_Complex_I;
                        gsp [i*nd+5] = creal(coeff_c2s[60])*gcart[6*i+0]
                                     + creal(coeff_c2s[63])*gcart[6*i+3]
                                     - cimag(coeff_c2s[61])*gcart[6*i+1]*_Complex_I;

                        gsp1[i*nd+0] = creal(coeff_c2s[6])*gcart[6*i+0]
                                     + creal(coeff_c2s[9])*gcart[6*i+3]
                                     - cimag(coeff_c2s[7])*gcart[6*i+1]*_Complex_I;
                        gsp1[i*nd+1] = creal(coeff_c2s[20])*gcart[6*i+2]
                                     - cimag(coeff_c2s[22])*gcart[6*i+4]*_Complex_I;
                        gsp1[i*nd+2] = creal(coeff_c2s[30])*gcart[6*i+0]
                                     + creal(coeff_c2s[33])*gcart[6*i+3]
                                     + creal(coeff_c2s[35])*gcart[6*i+5];
                        gsp1[i*nd+3] = creal(coeff_c2s[44])*gcart[6*i+2]
                                     - cimag(coeff_c2s[46])*gcart[6*i+4]*_Complex_I;
                        gsp1[i*nd+4] = creal(coeff_c2s[54])*gcart[6*i+0]
                                     + creal(coeff_c2s[57])*gcart[6*i+3]
                                     - cimag(coeff_c2s[55])*gcart[6*i+1]*_Complex_I;
                        gsp1[i*nd+5] = 0;
                }
        }
}
static void d_bra_cart2spinor_e1sf(double complex *gsp, int nket,
                                   double *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        double complex *gsp1 = gsp + nket * nd;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[2].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        // spin-up part
                        gsp [i*nd+0] = creal(coeff_c2s[0])*gcart[6*i+0]
                                     + creal(coeff_c2s[3])*gcart[6*i+3]
                                     - cimag(coeff_c2s[1])*gcart[6*i+1]*_Complex_I;
                        gsp [i*nd+1] = creal(coeff_c2s[14])*gcart[6*i+2]
                                     - cimag(coeff_c2s[16])*gcart[6*i+4]*_Complex_I;
                        gsp [i*nd+2] = creal(coeff_c2s[24])*gcart[6*i+0]
                                     + creal(coeff_c2s[27])*gcart[6*i+3]
                                     + creal(coeff_c2s[29])*gcart[6*i+5];
                        gsp [i*nd+3] = creal(coeff_c2s[38])*gcart[6*i+2]
                                     - cimag(coeff_c2s[40])*gcart[6*i+4]*_Complex_I;
                        // spin-down part
                        gsp1[i*nd+0] = creal(coeff_c2s[8])*gcart[6*i+2]
                                     - cimag(coeff_c2s[10])*gcart[6*i+4]*_Complex_I;
                        gsp1[i*nd+1] = creal(coeff_c2s[18])*gcart[6*i+0]
                                     + creal(coeff_c2s[21])*gcart[6*i+3]
                                     + creal(coeff_c2s[23])*gcart[6*i+5];
                        gsp1[i*nd+2] = creal(coeff_c2s[32])*gcart[6*i+2]
                                     - cimag(coeff_c2s[34])*gcart[6*i+4]*_Complex_I;
                        gsp1[i*nd+3] = creal(coeff_c2s[42])*gcart[6*i+0]
                                     + creal(coeff_c2s[45])*gcart[6*i+3]
                                     - cimag(coeff_c2s[43])*gcart[6*i+1]*_Complex_I;
                }
                gsp  += 4;
                gsp1 += 4;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[2].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = 0;
                        gsp [i*nd+1] = creal(coeff_c2s[12])*gcart[6*i+0]
                                     + creal(coeff_c2s[15])*gcart[6*i+3]
                                     - cimag(coeff_c2s[13])*gcart[6*i+1]*_Complex_I;
                        gsp [i*nd+2] = creal(coeff_c2s[26])*gcart[6*i+2]
                                     - cimag(coeff_c2s[28])*gcart[6*i+4]*_Complex_I;
                        gsp [i*nd+3] = creal(coeff_c2s[36])*gcart[6*i+0]
                                     + creal(coeff_c2s[39])*gcart[6*i+3]
                                     + creal(coeff_c2s[41])*gcart[6*i+5];
                        gsp [i*nd+4] = creal(coeff_c2s[50])*gcart[6*i+2]
                                     - cimag(coeff_c2s[52])*gcart[6*i+4]*_Complex_I;
                        gsp [i*nd+5] = creal(coeff_c2s[60])*gcart[6*i+0]
                                     + creal(coeff_c2s[63])*gcart[6*i+3]
                                     - cimag(coeff_c2s[61])*gcart[6*i+1]*_Complex_I;

                        gsp1[i*nd+0] = creal(coeff_c2s[6])*gcart[6*i+0]
                                     + creal(coeff_c2s[9])*gcart[6*i+3]
                                     - cimag(coeff_c2s[7])*gcart[6*i+1]*_Complex_I;
                        gsp1[i*nd+1] = creal(coeff_c2s[20])*gcart[6*i+2]
                                     - cimag(coeff_c2s[22])*gcart[6*i+4]*_Complex_I;
                        gsp1[i*nd+2] = creal(coeff_c2s[30])*gcart[6*i+0]
                                     + creal(coeff_c2s[33])*gcart[6*i+3]
                                     + creal(coeff_c2s[35])*gcart[6*i+5];
                        gsp1[i*nd+3] = creal(coeff_c2s[44])*gcart[6*i+2]
                                     - cimag(coeff_c2s[46])*gcart[6*i+4]*_Complex_I;
                        gsp1[i*nd+4] = creal(coeff_c2s[54])*gcart[6*i+0]
                                     + creal(coeff_c2s[57])*gcart[6*i+3]
                                     - cimag(coeff_c2s[55])*gcart[6*i+1]*_Complex_I;
                        gsp1[i*nd+5] = 0;
                }
        }
}
static void d_bra_cart2spinor_si(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        double complex *gcart1 = gcart + nket * 6;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[2].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        // spin-up part
                        gsp[i*nd+0] = creal(coeff_c2s[0])*gcart[6*i+0]
                                    + creal(coeff_c2s[3])*gcart[6*i+3]
                                    - cimag(coeff_c2s[1])*gcart[6*i+1]*_Complex_I;
                        gsp[i*nd+1] = creal(coeff_c2s[14])*gcart[6*i+2]
                                    - cimag(coeff_c2s[16])*gcart[6*i+4]*_Complex_I;
                        gsp[i*nd+2] = creal(coeff_c2s[24])*gcart[6*i+0]
                                    + creal(coeff_c2s[27])*gcart[6*i+3]
                                    + creal(coeff_c2s[29])*gcart[6*i+5];
                        gsp[i*nd+3] = creal(coeff_c2s[38])*gcart[6*i+2]
                                    - cimag(coeff_c2s[40])*gcart[6*i+4]*_Complex_I;
                        // spin-down part
                        gsp[i*nd+0]+= creal(coeff_c2s[8])*gcart1[6*i+2]
                                    - cimag(coeff_c2s[10])*gcart1[6*i+4]*_Complex_I;
                        gsp[i*nd+1]+= creal(coeff_c2s[18])*gcart1[6*i+0]
                                    + creal(coeff_c2s[21])*gcart1[6*i+3]
                                    + creal(coeff_c2s[23])*gcart1[6*i+5];
                        gsp[i*nd+2]+= creal(coeff_c2s[32])*gcart1[6*i+2]
                                    - cimag(coeff_c2s[34])*gcart1[6*i+4]*_Complex_I;
                        gsp[i*nd+3]+= creal(coeff_c2s[42])*gcart1[6*i+0]
                                    + creal(coeff_c2s[45])*gcart1[6*i+3]
                                    - cimag(coeff_c2s[43])*gcart1[6*i+1]*_Complex_I;
                }
                gsp += 4;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[2].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp[i*nd+0] = 0;
                        gsp[i*nd+1] = creal(coeff_c2s[12])*gcart[6*i+0]
                                    + creal(coeff_c2s[15])*gcart[6*i+3]
                                    - cimag(coeff_c2s[13])*gcart[6*i+1]*_Complex_I;
                        gsp[i*nd+2] = creal(coeff_c2s[26])*gcart[6*i+2]
                                    - cimag(coeff_c2s[28])*gcart[6*i+4]*_Complex_I;
                        gsp[i*nd+3] = creal(coeff_c2s[36])*gcart[6*i+0]
                                    + creal(coeff_c2s[39])*gcart[6*i+3]
                                    + creal(coeff_c2s[41])*gcart[6*i+5];
                        gsp[i*nd+4] = creal(coeff_c2s[50])*gcart[6*i+2]
                                    - cimag(coeff_c2s[52])*gcart[6*i+4]*_Complex_I;
                        gsp[i*nd+5] = creal(coeff_c2s[60])*gcart[6*i+0]
                                    + creal(coeff_c2s[63])*gcart[6*i+3]
                                    - cimag(coeff_c2s[61])*gcart[6*i+1]*_Complex_I;

                        gsp[i*nd+0]+= creal(coeff_c2s[6])*gcart1[6*i+0]
                                    + creal(coeff_c2s[9])*gcart1[6*i+3]
                                    - cimag(coeff_c2s[7])*gcart1[6*i+1]*_Complex_I;
                        gsp[i*nd+1]+= creal(coeff_c2s[20])*gcart1[6*i+2]
                                    - cimag(coeff_c2s[22])*gcart1[6*i+4]*_Complex_I;
                        gsp[i*nd+2]+= creal(coeff_c2s[30])*gcart1[6*i+0]
                                    + creal(coeff_c2s[33])*gcart1[6*i+3]
                                    + creal(coeff_c2s[35])*gcart1[6*i+5];
                        gsp[i*nd+3]+= creal(coeff_c2s[44])*gcart1[6*i+2]
                                    - cimag(coeff_c2s[46])*gcart1[6*i+4]*_Complex_I;
                        gsp[i*nd+4]+= creal(coeff_c2s[54])*gcart1[6*i+0]
                                    + creal(coeff_c2s[57])*gcart1[6*i+3]
                                    - cimag(coeff_c2s[55])*gcart1[6*i+1]*_Complex_I;
                        gsp[i*nd+5]+= 0;
                }
        }
}
static void d_ket_cart2spinor(double complex *gsp, int nbra,
                              double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[2].cart2j_lt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[0])*gcart[nbra* 0+i]
                                      + creal(coeff_c2s[3])*gcart[nbra* 3+i]
                                      + cimag(coeff_c2s[1])*gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[8])*gcart[nbra* 8+i]
                                      + cimag(coeff_c2s[10])*gcart[nbra*10+i]*_Complex_I;
                        gsp[1*nbra+i] = creal(coeff_c2s[14])*gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[16])*gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[18])*gcart[nbra* 6+i]
                                      + creal(coeff_c2s[21])*gcart[nbra* 9+i]
                                      + creal(coeff_c2s[23])*gcart[nbra*11+i];
                        gsp[2*nbra+i] = creal(coeff_c2s[24])*gcart[nbra* 0+i]
                                      + creal(coeff_c2s[27])*gcart[nbra* 3+i]
                                      + creal(coeff_c2s[29])*gcart[nbra* 5+i]
                                      + creal(coeff_c2s[32])*gcart[nbra* 8+i]
                                      + cimag(coeff_c2s[34])*gcart[nbra*10+i]*_Complex_I;
                        gsp[3*nbra+i] = creal(coeff_c2s[38])*gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[40])*gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[42])*gcart[nbra* 6+i]
                                      + creal(coeff_c2s[45])*gcart[nbra* 9+i]
                                      + cimag(coeff_c2s[43])*gcart[nbra* 7+i]*_Complex_I;
                }
                gsp += nbra * 4;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[2].cart2j_gt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[6])*gcart[nbra* 6+i]
                                      + creal(coeff_c2s[9])*gcart[nbra* 9+i]
                                      + cimag(coeff_c2s[7])*gcart[nbra* 7+i]*_Complex_I;
                        gsp[1*nbra+i] = creal(coeff_c2s[12])*gcart[nbra* 0+i]
                                      + creal(coeff_c2s[15])*gcart[nbra* 3+i]
                                      + cimag(coeff_c2s[13])*gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[20])*gcart[nbra* 8+i]
                                      + cimag(coeff_c2s[22])*gcart[nbra*10+i]*_Complex_I;
                        gsp[2*nbra+i] = creal(coeff_c2s[26])*gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[28])*gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[30])*gcart[nbra* 6+i]
                                      + creal(coeff_c2s[33])*gcart[nbra* 9+i]
                                      + creal(coeff_c2s[35])*gcart[nbra*11+i];
                        gsp[3*nbra+i] = creal(coeff_c2s[36])*gcart[nbra* 0+i]
                                      + creal(coeff_c2s[39])*gcart[nbra* 3+i]
                                      + creal(coeff_c2s[41])*gcart[nbra* 5+i]
                                      + creal(coeff_c2s[44])*gcart[nbra* 8+i]
                                      + cimag(coeff_c2s[46])*gcart[nbra*10+i]*_Complex_I;
                        gsp[4*nbra+i] = creal(coeff_c2s[50])*gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[52])*gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[54])*gcart[nbra* 6+i]
                                      + creal(coeff_c2s[57])*gcart[nbra* 9+i]
                                      + cimag(coeff_c2s[55])*gcart[nbra* 7+i]*_Complex_I;
                        gsp[5*nbra+i] = creal(coeff_c2s[60])*gcart[nbra* 0+i]
                                      + creal(coeff_c2s[63])*gcart[nbra* 3+i]
                                      + cimag(coeff_c2s[61])*gcart[nbra* 1+i]*_Complex_I;
                }
        }
}
static void d_iket_cart2spinor(double complex *gsp, int nbra,
                               double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[2].cart2j_lt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[0])*gcart[nbra* 0+i]*_Complex_I
                                      + creal(coeff_c2s[3])*gcart[nbra* 3+i]*_Complex_I
                                      - cimag(coeff_c2s[1])*gcart[nbra* 1+i]
                                      + creal(coeff_c2s[8])*gcart[nbra* 8+i]*_Complex_I
                                      - cimag(coeff_c2s[10])*gcart[nbra*10+i];
                        gsp[1*nbra+i] = creal(coeff_c2s[14])*gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[16])*gcart[nbra* 4+i]
                                      + creal(coeff_c2s[18])*gcart[nbra* 6+i]*_Complex_I
                                      + creal(coeff_c2s[21])*gcart[nbra* 9+i]*_Complex_I
                                      + creal(coeff_c2s[23])*gcart[nbra*11+i]*_Complex_I;
                        gsp[2*nbra+i] = creal(coeff_c2s[24])*gcart[nbra* 0+i]*_Complex_I
                                      + creal(coeff_c2s[27])*gcart[nbra* 3+i]*_Complex_I
                                      + creal(coeff_c2s[29])*gcart[nbra* 5+i]*_Complex_I
                                      + creal(coeff_c2s[32])*gcart[nbra* 8+i]*_Complex_I
                                      - cimag(coeff_c2s[34])*gcart[nbra*10+i];
                        gsp[3*nbra+i] = creal(coeff_c2s[38])*gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[40])*gcart[nbra* 4+i]
                                      + creal(coeff_c2s[42])*gcart[nbra* 6+i]*_Complex_I
                                      + creal(coeff_c2s[45])*gcart[nbra* 9+i]*_Complex_I
                                      - cimag(coeff_c2s[43])*gcart[nbra* 7+i];
                }
                gsp += nbra * 4;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[2].cart2j_gt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[6])*gcart[nbra* 6+i]*_Complex_I
                                      + creal(coeff_c2s[9])*gcart[nbra* 9+i]*_Complex_I
                                      - cimag(coeff_c2s[7])*gcart[nbra* 7+i];
                        gsp[1*nbra+i] = creal(coeff_c2s[12])*gcart[nbra* 0+i]*_Complex_I
                                      + creal(coeff_c2s[15])*gcart[nbra* 3+i]*_Complex_I
                                      - cimag(coeff_c2s[13])*gcart[nbra* 1+i]
                                      + creal(coeff_c2s[20])*gcart[nbra* 8+i]*_Complex_I
                                      - cimag(coeff_c2s[22])*gcart[nbra*10+i];
                        gsp[2*nbra+i] = creal(coeff_c2s[26])*gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[28])*gcart[nbra* 4+i]
                                      + creal(coeff_c2s[30])*gcart[nbra* 6+i]*_Complex_I
                                      + creal(coeff_c2s[33])*gcart[nbra* 9+i]*_Complex_I
                                      + creal(coeff_c2s[35])*gcart[nbra*11+i]*_Complex_I;
                        gsp[3*nbra+i] = creal(coeff_c2s[36])*gcart[nbra* 0+i]*_Complex_I
                                      + creal(coeff_c2s[39])*gcart[nbra* 3+i]*_Complex_I
                                      + creal(coeff_c2s[41])*gcart[nbra* 5+i]*_Complex_I
                                      + creal(coeff_c2s[44])*gcart[nbra* 8+i]*_Complex_I
                                      - cimag(coeff_c2s[46])*gcart[nbra*10+i];
                        gsp[4*nbra+i] = creal(coeff_c2s[50])*gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[52])*gcart[nbra* 4+i]
                                      + creal(coeff_c2s[54])*gcart[nbra* 6+i]*_Complex_I
                                      + creal(coeff_c2s[57])*gcart[nbra* 9+i]*_Complex_I
                                      - cimag(coeff_c2s[55])*gcart[nbra* 7+i];
                        gsp[5*nbra+i] = creal(coeff_c2s[60])*gcart[nbra* 0+i]*_Complex_I
                                      + creal(coeff_c2s[63])*gcart[nbra* 3+i]*_Complex_I
                                      - cimag(coeff_c2s[61])*gcart[nbra* 1+i];
                }
        }
}

static void f_bra_cart2spinor_sf(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        double complex *gsp1 = gsp + nket * nd;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[3].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = creal(coeff_c2s[0])*gcart[10*i+0]
                                     - cimag(coeff_c2s[1])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[3])*gcart[10*i+3]
                                     - cimag(coeff_c2s[6])*gcart[10*i+6]*_Complex_I;
                        gsp [i*nd+1] = creal(coeff_c2s[22])*gcart[10*i+2]
                                     - cimag(coeff_c2s[24])*gcart[10*i+4]*_Complex_I
                                     + creal(coeff_c2s[27])*gcart[10*i+7];
                        gsp [i*nd+2] = creal(coeff_c2s[40])*gcart[10*i+0]
                                     - cimag(coeff_c2s[41])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[43])*gcart[10*i+3]
                                     + creal(coeff_c2s[45])*gcart[10*i+5]
                                     - cimag(coeff_c2s[46])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[48])*gcart[10*i+8]*_Complex_I;
                        gsp [i*nd+3] = creal(coeff_c2s[62])*gcart[10*i+2]
                                     + creal(coeff_c2s[67])*gcart[10*i+7]
                                     + creal(coeff_c2s[69])*gcart[10*i+9];
                        gsp [i*nd+4] = creal(coeff_c2s[80])*gcart[10*i+0]
                                     - cimag(coeff_c2s[81])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[83])*gcart[10*i+3]
                                     + creal(coeff_c2s[85])*gcart[10*i+5]
                                     - cimag(coeff_c2s[86])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[88])*gcart[10*i+8]*_Complex_I;
                        gsp [i*nd+5] = creal(coeff_c2s[102])*gcart[10*i+2]
                                     - cimag(coeff_c2s[104])*gcart[10*i+4]*_Complex_I
                                     + creal(coeff_c2s[107])*gcart[10*i+7];

                        gsp1[i*nd+0] = creal(coeff_c2s[12])*gcart[10*i+2]
                                     - cimag(coeff_c2s[14])*gcart[10*i+4]*_Complex_I
                                     + creal(coeff_c2s[17])*gcart[10*i+7];
                        gsp1[i*nd+1] = creal(coeff_c2s[30])*gcart[10*i+0]
                                     - cimag(coeff_c2s[31])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[33])*gcart[10*i+3]
                                     + creal(coeff_c2s[35])*gcart[10*i+5]
                                     - cimag(coeff_c2s[36])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[38])*gcart[10*i+8]*_Complex_I;
                        gsp1[i*nd+2] = creal(coeff_c2s[52])*gcart[10*i+2]
                                     + creal(coeff_c2s[57])*gcart[10*i+7]
                                     + creal(coeff_c2s[59])*gcart[10*i+9];
                        gsp1[i*nd+3] = creal(coeff_c2s[70])*gcart[10*i+0]
                                     - cimag(coeff_c2s[71])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[73])*gcart[10*i+3]
                                     + creal(coeff_c2s[75])*gcart[10*i+5]
                                     - cimag(coeff_c2s[76])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[78])*gcart[10*i+8]*_Complex_I;
                        gsp1[i*nd+4] = creal(coeff_c2s[92])*gcart[10*i+2]
                                     - cimag(coeff_c2s[94])*gcart[10*i+4]*_Complex_I
                                     + creal(coeff_c2s[97])*gcart[10*i+7];
                        gsp1[i*nd+5] = creal(coeff_c2s[110])*gcart[10*i+0]
                                     - cimag(coeff_c2s[111])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[113])*gcart[10*i+3]
                                     - cimag(coeff_c2s[116])*gcart[10*i+6]*_Complex_I;
                }
                gsp  += 6;
                gsp1 += 6;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[3].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = 0;
                        gsp [i*nd+1] = creal(coeff_c2s[20])*gcart[10*i+0]
                                     - cimag(coeff_c2s[21])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[23])*gcart[10*i+3]
                                     - cimag(coeff_c2s[26])*gcart[10*i+6]*_Complex_I;
                        gsp [i*nd+2] = creal(coeff_c2s[42])*gcart[10*i+2]
                                     - cimag(coeff_c2s[44])*gcart[10*i+4]*_Complex_I
                                     + creal(coeff_c2s[47])*gcart[10*i+7];
                        gsp [i*nd+3] = creal(coeff_c2s[60])*gcart[10*i+0]
                                     - cimag(coeff_c2s[61])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[63])*gcart[10*i+3]
                                     + creal(coeff_c2s[65])*gcart[10*i+5]
                                     - cimag(coeff_c2s[66])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[68])*gcart[10*i+8]*_Complex_I;
                        gsp [i*nd+4] = creal(coeff_c2s[82])*gcart[10*i+2]
                                     + creal(coeff_c2s[87])*gcart[10*i+7]
                                     + creal(coeff_c2s[89])*gcart[10*i+9];
                        gsp [i*nd+5] = creal(coeff_c2s[100])*gcart[10*i+0]
                                     - cimag(coeff_c2s[101])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[103])*gcart[10*i+3]
                                     + creal(coeff_c2s[105])*gcart[10*i+5]
                                     - cimag(coeff_c2s[106])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[108])*gcart[10*i+8]*_Complex_I;
                        gsp [i*nd+6] = creal(coeff_c2s[122])*gcart[10*i+2]
                                     - cimag(coeff_c2s[124])*gcart[10*i+4]*_Complex_I
                                     + creal(coeff_c2s[127])*gcart[10*i+7];
                        gsp [i*nd+7] = creal(coeff_c2s[140])*gcart[10*i+0]
                                     - cimag(coeff_c2s[141])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[143])*gcart[10*i+3]
                                     - cimag(coeff_c2s[146])*gcart[10*i+6]*_Complex_I;

                        gsp1[i*nd+0] = creal(coeff_c2s[10])*gcart[10*i+0]
                                     - cimag(coeff_c2s[11])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[13])*gcart[10*i+3]
                                     - cimag(coeff_c2s[16])*gcart[10*i+6]*_Complex_I;
                        gsp1[i*nd+1] = creal(coeff_c2s[32])*gcart[10*i+2]
                                     - cimag(coeff_c2s[34])*gcart[10*i+4]*_Complex_I
                                     + creal(coeff_c2s[37])*gcart[10*i+7];
                        gsp1[i*nd+2] = creal(coeff_c2s[50])*gcart[10*i+0]
                                     - cimag(coeff_c2s[51])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[53])*gcart[10*i+3]
                                     + creal(coeff_c2s[55])*gcart[10*i+5]
                                     - cimag(coeff_c2s[56])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[58])*gcart[10*i+8]*_Complex_I;
                        gsp1[i*nd+3] = creal(coeff_c2s[72])*gcart[10*i+2]
                                     + creal(coeff_c2s[77])*gcart[10*i+7]
                                     + creal(coeff_c2s[79])*gcart[10*i+9];
                        gsp1[i*nd+4] = creal(coeff_c2s[90])*gcart[10*i+0]
                                     - cimag(coeff_c2s[91])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[93])*gcart[10*i+3]
                                     + creal(coeff_c2s[95])*gcart[10*i+5]
                                     - cimag(coeff_c2s[96])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[98])*gcart[10*i+8]*_Complex_I;
                        gsp1[i*nd+5] = creal(coeff_c2s[112])*gcart[10*i+2]
                                     - cimag(coeff_c2s[114])*gcart[10*i+4]*_Complex_I
                                     + creal(coeff_c2s[117])*gcart[10*i+7];
                        gsp1[i*nd+6] = creal(coeff_c2s[130])*gcart[10*i+0]
                                     - cimag(coeff_c2s[131])*gcart[10*i+1]*_Complex_I
                                     + creal(coeff_c2s[133])*gcart[10*i+3]
                                     - cimag(coeff_c2s[136])*gcart[10*i+6]*_Complex_I;
                        gsp1[i*nd+7] = 0;
                }
        }
}
static void f_bra_cart2spinor_e1sf(double complex *gsp, int nket,
                                   double *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        double complex *gsp1 = gsp + nket * nd;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[3].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = creal(coeff_c2s[0])*gcart[10*i+0]
                                     + creal(coeff_c2s[3])*gcart[10*i+3]
                                     - cimag(coeff_c2s[1])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[6])*gcart[10*i+6]*_Complex_I;
                        gsp [i*nd+1] = creal(coeff_c2s[22])*gcart[10*i+2]
                                     + creal(coeff_c2s[27])*gcart[10*i+7]
                                     - cimag(coeff_c2s[24])*gcart[10*i+4]*_Complex_I;
                        gsp [i*nd+2] = creal(coeff_c2s[40])*gcart[10*i+0]
                                     + creal(coeff_c2s[43])*gcart[10*i+3]
                                     + creal(coeff_c2s[45])*gcart[10*i+5]
                                     - cimag(coeff_c2s[41])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[46])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[48])*gcart[10*i+8]*_Complex_I;
                        gsp [i*nd+3] = creal(coeff_c2s[62])*gcart[10*i+2]
                                     + creal(coeff_c2s[67])*gcart[10*i+7]
                                     + creal(coeff_c2s[69])*gcart[10*i+9];
                        gsp [i*nd+4] = creal(coeff_c2s[80])*gcart[10*i+0]
                                     + creal(coeff_c2s[83])*gcart[10*i+3]
                                     + creal(coeff_c2s[85])*gcart[10*i+5]
                                     - cimag(coeff_c2s[81])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[86])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[88])*gcart[10*i+8]*_Complex_I;
                        gsp [i*nd+5] = creal(coeff_c2s[102])*gcart[10*i+2]
                                     + creal(coeff_c2s[107])*gcart[10*i+7]
                                     - cimag(coeff_c2s[104])*gcart[10*i+4]*_Complex_I;

                        gsp1[i*nd+0] = creal(coeff_c2s[12])*gcart[10*i+2]
                                     + creal(coeff_c2s[17])*gcart[10*i+7]
                                     - cimag(coeff_c2s[14])*gcart[10*i+4]*_Complex_I;
                        gsp1[i*nd+1] = creal(coeff_c2s[30])*gcart[10*i+0]
                                     + creal(coeff_c2s[33])*gcart[10*i+3]
                                     + creal(coeff_c2s[35])*gcart[10*i+5]
                                     - cimag(coeff_c2s[31])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[36])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[38])*gcart[10*i+8]*_Complex_I;
                        gsp1[i*nd+2] = creal(coeff_c2s[52])*gcart[10*i+2]
                                     + creal(coeff_c2s[57])*gcart[10*i+7]
                                     + creal(coeff_c2s[59])*gcart[10*i+9];
                        gsp1[i*nd+3] = creal(coeff_c2s[70])*gcart[10*i+0]
                                     + creal(coeff_c2s[73])*gcart[10*i+3]
                                     + creal(coeff_c2s[75])*gcart[10*i+5]
                                     - cimag(coeff_c2s[71])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[76])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[78])*gcart[10*i+8]*_Complex_I;
                        gsp1[i*nd+4] = creal(coeff_c2s[92])*gcart[10*i+2]
                                     + creal(coeff_c2s[97])*gcart[10*i+7]
                                     - cimag(coeff_c2s[94])*gcart[10*i+4]*_Complex_I;
                        gsp1[i*nd+5] = creal(coeff_c2s[110])*gcart[10*i+0]
                                     + creal(coeff_c2s[113])*gcart[10*i+3]
                                     - cimag(coeff_c2s[111])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[116])*gcart[10*i+6]*_Complex_I;
                }
                gsp  += 6;
                gsp1 += 6;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[3].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = 0;
                        gsp [i*nd+1] = creal(coeff_c2s[20])*gcart[10*i+0]
                                     + creal(coeff_c2s[23])*gcart[10*i+3]
                                     - cimag(coeff_c2s[21])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[26])*gcart[10*i+6]*_Complex_I;
                        gsp [i*nd+2] = creal(coeff_c2s[42])*gcart[10*i+2]
                                     + creal(coeff_c2s[47])*gcart[10*i+7]
                                     - cimag(coeff_c2s[44])*gcart[10*i+4]*_Complex_I;
                        gsp [i*nd+3] = creal(coeff_c2s[60])*gcart[10*i+0]
                                     + creal(coeff_c2s[63])*gcart[10*i+3]
                                     + creal(coeff_c2s[65])*gcart[10*i+5]
                                     - cimag(coeff_c2s[61])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[66])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[68])*gcart[10*i+8]*_Complex_I;
                        gsp [i*nd+4] = creal(coeff_c2s[82])*gcart[10*i+2]
                                     + creal(coeff_c2s[87])*gcart[10*i+7]
                                     + creal(coeff_c2s[89])*gcart[10*i+9];
                        gsp [i*nd+5] = creal(coeff_c2s[100])*gcart[10*i+0]
                                     + creal(coeff_c2s[103])*gcart[10*i+3]
                                     + creal(coeff_c2s[105])*gcart[10*i+5]
                                     - cimag(coeff_c2s[101])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[106])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[108])*gcart[10*i+8]*_Complex_I;
                        gsp [i*nd+6] = creal(coeff_c2s[122])*gcart[10*i+2]
                                     + creal(coeff_c2s[127])*gcart[10*i+7]
                                     - cimag(coeff_c2s[124])*gcart[10*i+4]*_Complex_I;
                        gsp [i*nd+7] = creal(coeff_c2s[140])*gcart[10*i+0]
                                     + creal(coeff_c2s[143])*gcart[10*i+3]
                                     - cimag(coeff_c2s[141])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[146])*gcart[10*i+6]*_Complex_I;

                        gsp1[i*nd+0] = creal(coeff_c2s[10])*gcart[10*i+0]
                                     + creal(coeff_c2s[13])*gcart[10*i+3]
                                     - cimag(coeff_c2s[11])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[16])*gcart[10*i+6]*_Complex_I;
                        gsp1[i*nd+1] = creal(coeff_c2s[32])*gcart[10*i+2]
                                     + creal(coeff_c2s[37])*gcart[10*i+7]
                                     - cimag(coeff_c2s[34])*gcart[10*i+4]*_Complex_I;
                        gsp1[i*nd+2] = creal(coeff_c2s[50])*gcart[10*i+0]
                                     + creal(coeff_c2s[53])*gcart[10*i+3]
                                     + creal(coeff_c2s[55])*gcart[10*i+5]
                                     - cimag(coeff_c2s[51])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[56])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[58])*gcart[10*i+8]*_Complex_I;
                        gsp1[i*nd+3] = creal(coeff_c2s[72])*gcart[10*i+2]
                                     + creal(coeff_c2s[77])*gcart[10*i+7]
                                     + creal(coeff_c2s[79])*gcart[10*i+9];
                        gsp1[i*nd+4] = creal(coeff_c2s[90])*gcart[10*i+0]
                                     + creal(coeff_c2s[93])*gcart[10*i+3]
                                     + creal(coeff_c2s[95])*gcart[10*i+5]
                                     - cimag(coeff_c2s[91])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[96])*gcart[10*i+6]*_Complex_I
                                     - cimag(coeff_c2s[98])*gcart[10*i+8]*_Complex_I;
                        gsp1[i*nd+5] = creal(coeff_c2s[112])*gcart[10*i+2]
                                     + creal(coeff_c2s[117])*gcart[10*i+7]
                                     - cimag(coeff_c2s[114])*gcart[10*i+4]*_Complex_I;
                        gsp1[i*nd+6] = creal(coeff_c2s[130])*gcart[10*i+0]
                                     + creal(coeff_c2s[133])*gcart[10*i+3]
                                     - cimag(coeff_c2s[131])*gcart[10*i+1]*_Complex_I
                                     - cimag(coeff_c2s[136])*gcart[10*i+6]*_Complex_I;
                        gsp1[i*nd+7] = 0;
                }
        }
}
static void f_bra_cart2spinor_si(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        double complex *gcart1 = gcart + nket * 10;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[3].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        gsp[i*nd+0] = creal(coeff_c2s[0])*gcart[10*i+0]
                                    - cimag(coeff_c2s[1])*gcart[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[3])*gcart[10*i+3]
                                    - cimag(coeff_c2s[6])*gcart[10*i+6]*_Complex_I;
                        gsp[i*nd+1] = creal(coeff_c2s[22])*gcart[10*i+2]
                                    - cimag(coeff_c2s[24])*gcart[10*i+4]*_Complex_I
                                    + creal(coeff_c2s[27])*gcart[10*i+7];
                        gsp[i*nd+2] = creal(coeff_c2s[40])*gcart[10*i+0]
                                    - cimag(coeff_c2s[41])*gcart[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[43])*gcart[10*i+3]
                                    + creal(coeff_c2s[45])*gcart[10*i+5]
                                    - cimag(coeff_c2s[46])*gcart[10*i+6]*_Complex_I
                                    - cimag(coeff_c2s[48])*gcart[10*i+8]*_Complex_I;
                        gsp[i*nd+3] = creal(coeff_c2s[62])*gcart[10*i+2]
                                    + creal(coeff_c2s[67])*gcart[10*i+7]
                                    + creal(coeff_c2s[69])*gcart[10*i+9];
                        gsp[i*nd+4] = creal(coeff_c2s[80])*gcart[10*i+0]
                                    - cimag(coeff_c2s[81])*gcart[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[83])*gcart[10*i+3]
                                    + creal(coeff_c2s[85])*gcart[10*i+5]
                                    - cimag(coeff_c2s[86])*gcart[10*i+6]*_Complex_I
                                    - cimag(coeff_c2s[88])*gcart[10*i+8]*_Complex_I;
                        gsp[i*nd+5] = creal(coeff_c2s[102])*gcart[10*i+2]
                                    - cimag(coeff_c2s[104])*gcart[10*i+4]*_Complex_I
                                    + creal(coeff_c2s[107])*gcart[10*i+7];
                }
                for (i = 0; i < nket; i++) {
                        gsp[i*nd+0]+= creal(coeff_c2s[12])*gcart1[10*i+2]
                                    - cimag(coeff_c2s[14])*gcart1[10*i+4]*_Complex_I
                                    + creal(coeff_c2s[17])*gcart1[10*i+7];
                        gsp[i*nd+1]+= creal(coeff_c2s[30])*gcart1[10*i+0]
                                    - cimag(coeff_c2s[31])*gcart1[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[33])*gcart1[10*i+3]
                                    + creal(coeff_c2s[35])*gcart1[10*i+5]
                                    - cimag(coeff_c2s[36])*gcart1[10*i+6]*_Complex_I
                                    - cimag(coeff_c2s[38])*gcart1[10*i+8]*_Complex_I;
                        gsp[i*nd+2]+= creal(coeff_c2s[52])*gcart1[10*i+2]
                                    + creal(coeff_c2s[57])*gcart1[10*i+7]
                                    + creal(coeff_c2s[59])*gcart1[10*i+9];
                        gsp[i*nd+3]+= creal(coeff_c2s[70])*gcart1[10*i+0]
                                    - cimag(coeff_c2s[71])*gcart1[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[73])*gcart1[10*i+3]
                                    + creal(coeff_c2s[75])*gcart1[10*i+5]
                                    - cimag(coeff_c2s[76])*gcart1[10*i+6]*_Complex_I
                                    - cimag(coeff_c2s[78])*gcart1[10*i+8]*_Complex_I;
                        gsp[i*nd+4]+= creal(coeff_c2s[92])*gcart1[10*i+2]
                                    - cimag(coeff_c2s[94])*gcart1[10*i+4]*_Complex_I
                                    + creal(coeff_c2s[97])*gcart1[10*i+7];
                        gsp[i*nd+5]+= creal(coeff_c2s[110])*gcart1[10*i+0]
                                    - cimag(coeff_c2s[111])*gcart1[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[113])*gcart1[10*i+3]
                                    - cimag(coeff_c2s[116])*gcart1[10*i+6]*_Complex_I;
                }
                gsp += 6;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[3].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp[i*nd+0] = 0;
                        gsp[i*nd+1] = creal(coeff_c2s[20])*gcart[10*i+0]
                                    - cimag(coeff_c2s[21])*gcart[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[23])*gcart[10*i+3]
                                    - cimag(coeff_c2s[26])*gcart[10*i+6]*_Complex_I;
                        gsp[i*nd+2] = creal(coeff_c2s[42])*gcart[10*i+2]
                                    - cimag(coeff_c2s[44])*gcart[10*i+4]*_Complex_I
                                    + creal(coeff_c2s[47])*gcart[10*i+7];
                        gsp[i*nd+3] = creal(coeff_c2s[60])*gcart[10*i+0]
                                    - cimag(coeff_c2s[61])*gcart[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[63])*gcart[10*i+3]
                                    + creal(coeff_c2s[65])*gcart[10*i+5]
                                    - cimag(coeff_c2s[66])*gcart[10*i+6]*_Complex_I
                                    - cimag(coeff_c2s[68])*gcart[10*i+8]*_Complex_I;
                        gsp[i*nd+4] = creal(coeff_c2s[82])*gcart[10*i+2]
                                    + creal(coeff_c2s[87])*gcart[10*i+7]
                                    + creal(coeff_c2s[89])*gcart[10*i+9];
                        gsp[i*nd+5] = creal(coeff_c2s[100])*gcart[10*i+0]
                                    - cimag(coeff_c2s[101])*gcart[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[103])*gcart[10*i+3]
                                    + creal(coeff_c2s[105])*gcart[10*i+5]
                                    - cimag(coeff_c2s[106])*gcart[10*i+6]*_Complex_I
                                    - cimag(coeff_c2s[108])*gcart[10*i+8]*_Complex_I;
                        gsp[i*nd+6] = creal(coeff_c2s[122])*gcart[10*i+2]
                                    - cimag(coeff_c2s[124])*gcart[10*i+4]*_Complex_I
                                    + creal(coeff_c2s[127])*gcart[10*i+7];
                        gsp[i*nd+7] = creal(coeff_c2s[140])*gcart[10*i+0]
                                    - cimag(coeff_c2s[141])*gcart[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[143])*gcart[10*i+3]
                                    - cimag(coeff_c2s[146])*gcart[10*i+6]*_Complex_I;
                }
                for (i = 0; i < nket; i++) {
                        gsp[i*nd+0]+= creal(coeff_c2s[10])*gcart1[10*i+0]
                                    - cimag(coeff_c2s[11])*gcart1[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[13])*gcart1[10*i+3]
                                    - cimag(coeff_c2s[16])*gcart1[10*i+6]*_Complex_I;
                        gsp[i*nd+1]+= creal(coeff_c2s[32])*gcart1[10*i+2]
                                    - cimag(coeff_c2s[34])*gcart1[10*i+4]*_Complex_I
                                    + creal(coeff_c2s[37])*gcart1[10*i+7];
                        gsp[i*nd+2]+= creal(coeff_c2s[50])*gcart1[10*i+0]
                                    - cimag(coeff_c2s[51])*gcart1[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[53])*gcart1[10*i+3]
                                    + creal(coeff_c2s[55])*gcart1[10*i+5]
                                    - cimag(coeff_c2s[56])*gcart1[10*i+6]*_Complex_I
                                    - cimag(coeff_c2s[58])*gcart1[10*i+8]*_Complex_I;
                        gsp[i*nd+3]+= creal(coeff_c2s[72])*gcart1[10*i+2]
                                    + creal(coeff_c2s[77])*gcart1[10*i+7]
                                    + creal(coeff_c2s[79])*gcart1[10*i+9];
                        gsp[i*nd+4]+= creal(coeff_c2s[90])*gcart1[10*i+0]
                                    - cimag(coeff_c2s[91])*gcart1[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[93])*gcart1[10*i+3]
                                    + creal(coeff_c2s[95])*gcart1[10*i+5]
                                    - cimag(coeff_c2s[96])*gcart1[10*i+6]*_Complex_I
                                    - cimag(coeff_c2s[98])*gcart1[10*i+8]*_Complex_I;
                        gsp[i*nd+5]+= creal(coeff_c2s[112])*gcart1[10*i+2]
                                    - cimag(coeff_c2s[114])*gcart1[10*i+4]*_Complex_I
                                    + creal(coeff_c2s[117])*gcart1[10*i+7];
                        gsp[i*nd+6]+= creal(coeff_c2s[130])*gcart1[10*i+0]
                                    - cimag(coeff_c2s[131])*gcart1[10*i+1]*_Complex_I
                                    + creal(coeff_c2s[133])*gcart1[10*i+3]
                                    - cimag(coeff_c2s[136])*gcart1[10*i+6]*_Complex_I;
                        gsp[i*nd+7]+= 0;
                }
        }
}
static void f_ket_cart2spinor(double complex *gsp, int nbra,
                              double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[3].cart2j_lt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[0])*gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[1])*gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[3])*gcart[nbra* 3+i]
                                      + cimag(coeff_c2s[6])*gcart[nbra* 6+i]*_Complex_I
                                      + creal(coeff_c2s[12])*gcart[nbra*12+i]
                                      + cimag(coeff_c2s[14])*gcart[nbra*14+i]*_Complex_I
                                      + creal(coeff_c2s[17])*gcart[nbra*17+i];
                        gsp[1*nbra+i] = creal(coeff_c2s[22])*gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[24])*gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[27])*gcart[nbra* 7+i]
                                      + creal(coeff_c2s[30])*gcart[nbra*10+i]
                                      + cimag(coeff_c2s[31])*gcart[nbra*11+i]*_Complex_I
                                      + creal(coeff_c2s[33])*gcart[nbra*13+i]
                                      + creal(coeff_c2s[35])*gcart[nbra*15+i]
                                      + cimag(coeff_c2s[36])*gcart[nbra*16+i]*_Complex_I
                                      + cimag(coeff_c2s[38])*gcart[nbra*18+i]*_Complex_I;
                        gsp[2*nbra+i] = creal(coeff_c2s[40])*gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[41])*gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[43])*gcart[nbra* 3+i]
                                      + creal(coeff_c2s[45])*gcart[nbra* 5+i]
                                      + cimag(coeff_c2s[46])*gcart[nbra* 6+i]*_Complex_I
                                      + cimag(coeff_c2s[48])*gcart[nbra* 8+i]*_Complex_I
                                      + creal(coeff_c2s[52])*gcart[nbra*12+i]
                                      + creal(coeff_c2s[57])*gcart[nbra*17+i]
                                      + creal(coeff_c2s[59])*gcart[nbra*19+i];
                        gsp[3*nbra+i] = creal(coeff_c2s[62])*gcart[nbra* 2+i]
                                      + creal(coeff_c2s[67])*gcart[nbra* 7+i]
                                      + creal(coeff_c2s[69])*gcart[nbra* 9+i]
                                      + creal(coeff_c2s[70])*gcart[nbra*10+i]
                                      + cimag(coeff_c2s[71])*gcart[nbra*11+i]*_Complex_I
                                      + creal(coeff_c2s[73])*gcart[nbra*13+i]
                                      + creal(coeff_c2s[75])*gcart[nbra*15+i]
                                      + cimag(coeff_c2s[76])*gcart[nbra*16+i]*_Complex_I
                                      + cimag(coeff_c2s[78])*gcart[nbra*18+i]*_Complex_I;
                        gsp[4*nbra+i] = creal(coeff_c2s[80])*gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[81])*gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[83])*gcart[nbra* 3+i]
                                      + creal(coeff_c2s[85])*gcart[nbra* 5+i]
                                      + cimag(coeff_c2s[86])*gcart[nbra* 6+i]*_Complex_I
                                      + cimag(coeff_c2s[88])*gcart[nbra* 8+i]*_Complex_I
                                      + creal(coeff_c2s[92])*gcart[nbra*12+i]
                                      + cimag(coeff_c2s[94])*gcart[nbra*14+i]*_Complex_I
                                      + creal(coeff_c2s[97])*gcart[nbra*17+i];
                        gsp[5*nbra+i] = creal(coeff_c2s[102])*gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[104])*gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[107])*gcart[nbra* 7+i]
                                      + creal(coeff_c2s[110])*gcart[nbra*10+i]
                                      + cimag(coeff_c2s[111])*gcart[nbra*11+i]*_Complex_I
                                      + creal(coeff_c2s[113])*gcart[nbra*13+i]
                                      + cimag(coeff_c2s[116])*gcart[nbra*16+i]*_Complex_I;
                }
                gsp += nbra * 6;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[3].cart2j_gt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[10])*gcart[nbra*10+i]
                                      + cimag(coeff_c2s[11])*gcart[nbra*11+i]*_Complex_I
                                      + creal(coeff_c2s[13])*gcart[nbra*13+i]
                                      + cimag(coeff_c2s[16])*gcart[nbra*16+i]*_Complex_I;
                        gsp[1*nbra+i] = creal(coeff_c2s[20])*gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[21])*gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[23])*gcart[nbra* 3+i]
                                      + cimag(coeff_c2s[26])*gcart[nbra* 6+i]*_Complex_I
                                      + creal(coeff_c2s[32])*gcart[nbra*12+i]
                                      + cimag(coeff_c2s[34])*gcart[nbra*14+i]*_Complex_I
                                      + creal(coeff_c2s[37])*gcart[nbra*17+i];
                        gsp[2*nbra+i] = creal(coeff_c2s[42])*gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[44])*gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[47])*gcart[nbra* 7+i]
                                      + creal(coeff_c2s[50])*gcart[nbra*10+i]
                                      + cimag(coeff_c2s[51])*gcart[nbra*11+i]*_Complex_I
                                      + creal(coeff_c2s[53])*gcart[nbra*13+i]
                                      + creal(coeff_c2s[55])*gcart[nbra*15+i]
                                      + cimag(coeff_c2s[56])*gcart[nbra*16+i]*_Complex_I
                                      + cimag(coeff_c2s[58])*gcart[nbra*18+i]*_Complex_I;
                        gsp[3*nbra+i] = creal(coeff_c2s[60])*gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[61])*gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[63])*gcart[nbra* 3+i]
                                      + creal(coeff_c2s[65])*gcart[nbra* 5+i]
                                      + cimag(coeff_c2s[66])*gcart[nbra* 6+i]*_Complex_I
                                      + cimag(coeff_c2s[68])*gcart[nbra* 8+i]*_Complex_I
                                      + creal(coeff_c2s[72])*gcart[nbra*12+i]
                                      + creal(coeff_c2s[77])*gcart[nbra*17+i]
                                      + creal(coeff_c2s[79])*gcart[nbra*19+i];
                        gsp[4*nbra+i] = creal(coeff_c2s[82])*gcart[nbra* 2+i]
                                      + creal(coeff_c2s[87])*gcart[nbra* 7+i]
                                      + creal(coeff_c2s[89])*gcart[nbra* 9+i]
                                      + creal(coeff_c2s[90])*gcart[nbra*10+i]
                                      + cimag(coeff_c2s[91])*gcart[nbra*11+i]*_Complex_I
                                      + creal(coeff_c2s[93])*gcart[nbra*13+i]
                                      + creal(coeff_c2s[95])*gcart[nbra*15+i]
                                      + cimag(coeff_c2s[96])*gcart[nbra*16+i]*_Complex_I
                                      + cimag(coeff_c2s[98])*gcart[nbra*18+i]*_Complex_I;
                        gsp[5*nbra+i] = creal(coeff_c2s[100])*gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[101])*gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[103])*gcart[nbra* 3+i]
                                      + creal(coeff_c2s[105])*gcart[nbra* 5+i]
                                      + cimag(coeff_c2s[106])*gcart[nbra* 6+i]*_Complex_I
                                      + cimag(coeff_c2s[108])*gcart[nbra* 8+i]*_Complex_I
                                      + creal(coeff_c2s[112])*gcart[nbra*12+i]
                                      + cimag(coeff_c2s[114])*gcart[nbra*14+i]*_Complex_I
                                      + creal(coeff_c2s[117])*gcart[nbra*17+i];
                        gsp[6*nbra+i] = creal(coeff_c2s[122])*gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[124])*gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[127])*gcart[nbra* 7+i]
                                      + creal(coeff_c2s[130])*gcart[nbra*10+i]
                                      + cimag(coeff_c2s[131])*gcart[nbra*11+i]*_Complex_I
                                      + creal(coeff_c2s[133])*gcart[nbra*13+i]
                                      + cimag(coeff_c2s[136])*gcart[nbra*16+i]*_Complex_I;
                        gsp[7*nbra+i] = creal(coeff_c2s[140])*gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[141])*gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[143])*gcart[nbra* 3+i]
                                      + cimag(coeff_c2s[146])*gcart[nbra* 6+i]*_Complex_I;
                }
        }
}
static void f_iket_cart2spinor(double complex *gsp, int nbra,
                               double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[3].cart2j_lt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[0])*gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[1])*gcart[nbra* 1+i]
                                      + creal(coeff_c2s[3])*gcart[nbra* 3+i]*_Complex_I
                                      - cimag(coeff_c2s[6])*gcart[nbra* 6+i]
                                      + creal(coeff_c2s[12])*gcart[nbra*12+i]*_Complex_I
                                      - cimag(coeff_c2s[14])*gcart[nbra*14+i]
                                      + creal(coeff_c2s[17])*gcart[nbra*17+i]*_Complex_I;
                        gsp[1*nbra+i] = creal(coeff_c2s[22])*gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[24])*gcart[nbra* 4+i]
                                      + creal(coeff_c2s[27])*gcart[nbra* 7+i]*_Complex_I
                                      + creal(coeff_c2s[30])*gcart[nbra*10+i]*_Complex_I
                                      - cimag(coeff_c2s[31])*gcart[nbra*11+i]
                                      + creal(coeff_c2s[33])*gcart[nbra*13+i]*_Complex_I
                                      + creal(coeff_c2s[35])*gcart[nbra*15+i]*_Complex_I
                                      - cimag(coeff_c2s[36])*gcart[nbra*16+i]
                                      - cimag(coeff_c2s[38])*gcart[nbra*18+i];
                        gsp[2*nbra+i] = creal(coeff_c2s[40])*gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[41])*gcart[nbra* 1+i]
                                      + creal(coeff_c2s[43])*gcart[nbra* 3+i]*_Complex_I
                                      + creal(coeff_c2s[45])*gcart[nbra* 5+i]*_Complex_I
                                      - cimag(coeff_c2s[46])*gcart[nbra* 6+i]
                                      - cimag(coeff_c2s[48])*gcart[nbra* 8+i]
                                      + creal(coeff_c2s[52])*gcart[nbra*12+i]*_Complex_I
                                      + creal(coeff_c2s[57])*gcart[nbra*17+i]*_Complex_I
                                      + creal(coeff_c2s[59])*gcart[nbra*19+i]*_Complex_I;
                        gsp[3*nbra+i] = creal(coeff_c2s[62])*gcart[nbra* 2+i]*_Complex_I
                                      + creal(coeff_c2s[67])*gcart[nbra* 7+i]*_Complex_I
                                      + creal(coeff_c2s[69])*gcart[nbra* 9+i]*_Complex_I
                                      + creal(coeff_c2s[70])*gcart[nbra*10+i]*_Complex_I
                                      - cimag(coeff_c2s[71])*gcart[nbra*11+i]
                                      + creal(coeff_c2s[73])*gcart[nbra*13+i]*_Complex_I
                                      + creal(coeff_c2s[75])*gcart[nbra*15+i]*_Complex_I
                                      - cimag(coeff_c2s[76])*gcart[nbra*16+i]
                                      - cimag(coeff_c2s[78])*gcart[nbra*18+i];
                        gsp[4*nbra+i] = creal(coeff_c2s[80])*gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[81])*gcart[nbra* 1+i]
                                      + creal(coeff_c2s[83])*gcart[nbra* 3+i]*_Complex_I
                                      + creal(coeff_c2s[85])*gcart[nbra* 5+i]*_Complex_I
                                      - cimag(coeff_c2s[86])*gcart[nbra* 6+i]
                                      - cimag(coeff_c2s[88])*gcart[nbra* 8+i]
                                      + creal(coeff_c2s[92])*gcart[nbra*12+i]*_Complex_I
                                      - cimag(coeff_c2s[94])*gcart[nbra*14+i]
                                      + creal(coeff_c2s[97])*gcart[nbra*17+i]*_Complex_I;
                        gsp[5*nbra+i] = creal(coeff_c2s[102])*gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[104])*gcart[nbra* 4+i]
                                      + creal(coeff_c2s[107])*gcart[nbra* 7+i]*_Complex_I
                                      + creal(coeff_c2s[110])*gcart[nbra*10+i]*_Complex_I
                                      - cimag(coeff_c2s[111])*gcart[nbra*11+i]
                                      + creal(coeff_c2s[113])*gcart[nbra*13+i]*_Complex_I
                                      - cimag(coeff_c2s[116])*gcart[nbra*16+i];
                }
                gsp += nbra * 6;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[3].cart2j_gt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[10])*gcart[nbra*10+i]*_Complex_I
                                      - cimag(coeff_c2s[11])*gcart[nbra*11+i]
                                      + creal(coeff_c2s[13])*gcart[nbra*13+i]*_Complex_I
                                      - cimag(coeff_c2s[16])*gcart[nbra*16+i];
                        gsp[1*nbra+i] = creal(coeff_c2s[20])*gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[21])*gcart[nbra* 1+i]
                                      + creal(coeff_c2s[23])*gcart[nbra* 3+i]*_Complex_I
                                      - cimag(coeff_c2s[26])*gcart[nbra* 6+i]
                                      + creal(coeff_c2s[32])*gcart[nbra*12+i]*_Complex_I
                                      - cimag(coeff_c2s[34])*gcart[nbra*14+i]
                                      + creal(coeff_c2s[37])*gcart[nbra*17+i]*_Complex_I;
                        gsp[2*nbra+i] = creal(coeff_c2s[42])*gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[44])*gcart[nbra* 4+i]
                                      + creal(coeff_c2s[47])*gcart[nbra* 7+i]*_Complex_I
                                      + creal(coeff_c2s[50])*gcart[nbra*10+i]*_Complex_I
                                      - cimag(coeff_c2s[51])*gcart[nbra*11+i]
                                      + creal(coeff_c2s[53])*gcart[nbra*13+i]*_Complex_I
                                      + creal(coeff_c2s[55])*gcart[nbra*15+i]*_Complex_I
                                      - cimag(coeff_c2s[56])*gcart[nbra*16+i]
                                      - cimag(coeff_c2s[58])*gcart[nbra*18+i];
                        gsp[3*nbra+i] = creal(coeff_c2s[60])*gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[61])*gcart[nbra* 1+i]
                                      + creal(coeff_c2s[63])*gcart[nbra* 3+i]*_Complex_I
                                      + creal(coeff_c2s[65])*gcart[nbra* 5+i]*_Complex_I
                                      - cimag(coeff_c2s[66])*gcart[nbra* 6+i]
                                      - cimag(coeff_c2s[68])*gcart[nbra* 8+i]
                                      + creal(coeff_c2s[72])*gcart[nbra*12+i]*_Complex_I
                                      + creal(coeff_c2s[77])*gcart[nbra*17+i]*_Complex_I
                                      + creal(coeff_c2s[79])*gcart[nbra*19+i]*_Complex_I;
                        gsp[4*nbra+i] = creal(coeff_c2s[82])*gcart[nbra* 2+i]*_Complex_I
                                      + creal(coeff_c2s[87])*gcart[nbra* 7+i]*_Complex_I
                                      + creal(coeff_c2s[89])*gcart[nbra* 9+i]*_Complex_I
                                      + creal(coeff_c2s[90])*gcart[nbra*10+i]*_Complex_I
                                      - cimag(coeff_c2s[91])*gcart[nbra*11+i]
                                      + creal(coeff_c2s[93])*gcart[nbra*13+i]*_Complex_I
                                      + creal(coeff_c2s[95])*gcart[nbra*15+i]*_Complex_I
                                      - cimag(coeff_c2s[96])*gcart[nbra*16+i]
                                      - cimag(coeff_c2s[98])*gcart[nbra*18+i];
                        gsp[5*nbra+i] = creal(coeff_c2s[100])*gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[101])*gcart[nbra* 1+i]
                                      + creal(coeff_c2s[103])*gcart[nbra* 3+i]*_Complex_I
                                      + creal(coeff_c2s[105])*gcart[nbra* 5+i]*_Complex_I
                                      - cimag(coeff_c2s[106])*gcart[nbra* 6+i]
                                      - cimag(coeff_c2s[108])*gcart[nbra* 8+i]
                                      + creal(coeff_c2s[112])*gcart[nbra*12+i]*_Complex_I
                                      - cimag(coeff_c2s[114])*gcart[nbra*14+i]
                                      + creal(coeff_c2s[117])*gcart[nbra*17+i]*_Complex_I;
                        gsp[6*nbra+i] = creal(coeff_c2s[122])*gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[124])*gcart[nbra* 4+i]
                                      + creal(coeff_c2s[127])*gcart[nbra* 7+i]*_Complex_I
                                      + creal(coeff_c2s[130])*gcart[nbra*10+i]*_Complex_I
                                      - cimag(coeff_c2s[131])*gcart[nbra*11+i]
                                      + creal(coeff_c2s[133])*gcart[nbra*13+i]*_Complex_I
                                      - cimag(coeff_c2s[136])*gcart[nbra*16+i];
                        gsp[7*nbra+i] = creal(coeff_c2s[140])*gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[141])*gcart[nbra* 1+i]
                                      + creal(coeff_c2s[143])*gcart[nbra* 3+i]*_Complex_I
                                      - cimag(coeff_c2s[146])*gcart[nbra* 6+i];
                }
        }
}

static void g_bra_cart2spinor_sf(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        double complex *gsp1 = gsp + nket * nd;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[4].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = creal(coeff_c2s[0]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[1]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[3]) * gcart[15*i+ 3]
                                     - cimag(coeff_c2s[6]) * gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[10]) * gcart[15*i+10];
                        gsp [i*nd+1] = creal(coeff_c2s[32]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[34]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[37]) * gcart[15*i+ 7]
                                     - cimag(coeff_c2s[41]) * gcart[15*i+11]*_Complex_I;
                        gsp [i*nd+2] = creal(coeff_c2s[60]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[61]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[65]) * gcart[15*i+ 5]
                                     - cimag(coeff_c2s[66]) * gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[68]) * gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[70]) * gcart[15*i+10]
                                     + creal(coeff_c2s[72]) * gcart[15*i+12];
                        gsp [i*nd+3] = creal(coeff_c2s[92]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[94]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[97]) * gcart[15*i+ 7]
                                     + creal(coeff_c2s[99]) * gcart[15*i+ 9]
                                     - cimag(coeff_c2s[101]) * gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[103]) * gcart[15*i+13]*_Complex_I;
                        gsp [i*nd+4] = creal(coeff_c2s[120]) * gcart[15*i+ 0]
                                     + creal(coeff_c2s[123]) * gcart[15*i+ 3]
                                     + creal(coeff_c2s[125]) * gcart[15*i+ 5]
                                     + creal(coeff_c2s[130]) * gcart[15*i+10]
                                     + creal(coeff_c2s[132]) * gcart[15*i+12]
                                     + creal(coeff_c2s[134]) * gcart[15*i+14];
                        gsp [i*nd+5] = creal(coeff_c2s[152]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[154]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[157]) * gcart[15*i+ 7]
                                     + creal(coeff_c2s[159]) * gcart[15*i+ 9]
                                     - cimag(coeff_c2s[161]) * gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[163]) * gcart[15*i+13]*_Complex_I;
                        gsp [i*nd+6] = creal(coeff_c2s[180]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[181]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[185]) * gcart[15*i+ 5]
                                     - cimag(coeff_c2s[186]) * gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[188]) * gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[190]) * gcart[15*i+10]
                                     + creal(coeff_c2s[192]) * gcart[15*i+12];
                        gsp [i*nd+7] = creal(coeff_c2s[212]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[214]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[217]) * gcart[15*i+ 7]
                                     - cimag(coeff_c2s[221]) * gcart[15*i+11]*_Complex_I;

                        gsp1[i*nd+0] = creal(coeff_c2s[17]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[19]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[22]) * gcart[15*i+ 7]
                                     - cimag(coeff_c2s[26]) * gcart[15*i+11]*_Complex_I;
                        gsp1[i*nd+1] = creal(coeff_c2s[45]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[46]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[50]) * gcart[15*i+ 5]
                                     - cimag(coeff_c2s[51]) * gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[53]) * gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[55]) * gcart[15*i+10]
                                     + creal(coeff_c2s[57]) * gcart[15*i+12];
                        gsp1[i*nd+2] = creal(coeff_c2s[77]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[79]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[82]) * gcart[15*i+ 7]
                                     + creal(coeff_c2s[84]) * gcart[15*i+ 9]
                                     - cimag(coeff_c2s[86]) * gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[88]) * gcart[15*i+13]*_Complex_I;
                        gsp1[i*nd+3] = creal(coeff_c2s[105]) * gcart[15*i+ 0]
                                     + creal(coeff_c2s[108]) * gcart[15*i+ 3]
                                     + creal(coeff_c2s[110]) * gcart[15*i+ 5]
                                     + creal(coeff_c2s[115]) * gcart[15*i+10]
                                     + creal(coeff_c2s[117]) * gcart[15*i+12]
                                     + creal(coeff_c2s[119]) * gcart[15*i+14];
                        gsp1[i*nd+4] = creal(coeff_c2s[137]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[139]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[142]) * gcart[15*i+ 7]
                                     + creal(coeff_c2s[144]) * gcart[15*i+ 9]
                                     - cimag(coeff_c2s[146]) * gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[148]) * gcart[15*i+13]*_Complex_I;
                        gsp1[i*nd+5] = creal(coeff_c2s[165]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[166]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[170]) * gcart[15*i+ 5]
                                     - cimag(coeff_c2s[171]) * gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[173]) * gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[175]) * gcart[15*i+10]
                                     + creal(coeff_c2s[177]) * gcart[15*i+12];
                        gsp1[i*nd+6] = creal(coeff_c2s[197]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[199]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[202]) * gcart[15*i+ 7]
                                     - cimag(coeff_c2s[206]) * gcart[15*i+11]*_Complex_I;
                        gsp1[i*nd+7] = creal(coeff_c2s[225]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[226]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[228]) * gcart[15*i+ 3]
                                     - cimag(coeff_c2s[231]) * gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[235]) * gcart[15*i+10];
                }
                gsp  += 8;
                gsp1 += 8;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[4].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = 0;
                        gsp [i*nd+1] = creal(coeff_c2s[30]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[31]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[33]) * gcart[15*i+ 3]
                                     - cimag(coeff_c2s[36]) * gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[40]) * gcart[15*i+10];
                        gsp [i*nd+2] = creal(coeff_c2s[62]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[64]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[67]) * gcart[15*i+ 7]
                                     - cimag(coeff_c2s[71]) * gcart[15*i+11]*_Complex_I;
                        gsp [i*nd+3] = creal(coeff_c2s[90]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[91]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[95]) * gcart[15*i+ 5]
                                     - cimag(coeff_c2s[96]) * gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[98]) * gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[100]) * gcart[15*i+10]
                                     + creal(coeff_c2s[102]) * gcart[15*i+12];
                        gsp [i*nd+4] = creal(coeff_c2s[122]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[124]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[127]) * gcart[15*i+ 7]
                                     + creal(coeff_c2s[129]) * gcart[15*i+ 9]
                                     - cimag(coeff_c2s[131]) * gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[133]) * gcart[15*i+13]*_Complex_I;
                        gsp [i*nd+5] = creal(coeff_c2s[150]) * gcart[15*i+ 0]
                                     + creal(coeff_c2s[153]) * gcart[15*i+ 3]
                                     + creal(coeff_c2s[155]) * gcart[15*i+ 5]
                                     + creal(coeff_c2s[160]) * gcart[15*i+10]
                                     + creal(coeff_c2s[162]) * gcart[15*i+12]
                                     + creal(coeff_c2s[164]) * gcart[15*i+14];
                        gsp [i*nd+6] = creal(coeff_c2s[182]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[184]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[187]) * gcart[15*i+ 7]
                                     + creal(coeff_c2s[189]) * gcart[15*i+ 9]
                                     - cimag(coeff_c2s[191]) * gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[193]) * gcart[15*i+13]*_Complex_I;
                        gsp [i*nd+7] = creal(coeff_c2s[210]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[211]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[215]) * gcart[15*i+ 5]
                                     - cimag(coeff_c2s[216]) * gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[218]) * gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[220]) * gcart[15*i+10]
                                     + creal(coeff_c2s[222]) * gcart[15*i+12];
                        gsp [i*nd+8] = creal(coeff_c2s[242]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[244]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[247]) * gcart[15*i+ 7]
                                     - cimag(coeff_c2s[251]) * gcart[15*i+11]*_Complex_I;
                        gsp [i*nd+9] = creal(coeff_c2s[270]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[271]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[273]) * gcart[15*i+ 3]
                                     - cimag(coeff_c2s[276]) * gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[280]) * gcart[15*i+10];

                        gsp1[i*nd+0] = creal(coeff_c2s[15]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[16]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[18]) * gcart[15*i+ 3]
                                     - cimag(coeff_c2s[21]) * gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[25]) * gcart[15*i+10];
                        gsp1[i*nd+1] = creal(coeff_c2s[47]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[49]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[52]) * gcart[15*i+ 7]
                                     - cimag(coeff_c2s[56]) * gcart[15*i+11]*_Complex_I;
                        gsp1[i*nd+2] = creal(coeff_c2s[75]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[76]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[80]) * gcart[15*i+ 5]
                                     - cimag(coeff_c2s[81]) * gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[83]) * gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[85]) * gcart[15*i+10]
                                     + creal(coeff_c2s[87]) * gcart[15*i+12];
                        gsp1[i*nd+3] = creal(coeff_c2s[107]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[109]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[112]) * gcart[15*i+ 7]
                                     + creal(coeff_c2s[114]) * gcart[15*i+ 9]
                                     - cimag(coeff_c2s[116]) * gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[118]) * gcart[15*i+13]*_Complex_I;
                        gsp1[i*nd+4] = creal(coeff_c2s[135]) * gcart[15*i+ 0]
                                     + creal(coeff_c2s[138]) * gcart[15*i+ 3]
                                     + creal(coeff_c2s[140]) * gcart[15*i+ 5]
                                     + creal(coeff_c2s[145]) * gcart[15*i+10]
                                     + creal(coeff_c2s[147]) * gcart[15*i+12]
                                     + creal(coeff_c2s[149]) * gcart[15*i+14];
                        gsp1[i*nd+5] = creal(coeff_c2s[167]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[169]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[172]) * gcart[15*i+ 7]
                                     + creal(coeff_c2s[174]) * gcart[15*i+ 9]
                                     - cimag(coeff_c2s[176]) * gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[178]) * gcart[15*i+13]*_Complex_I;
                        gsp1[i*nd+6] = creal(coeff_c2s[195]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[196]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[200]) * gcart[15*i+ 5]
                                     - cimag(coeff_c2s[201]) * gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[203]) * gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[205]) * gcart[15*i+10]
                                     + creal(coeff_c2s[207]) * gcart[15*i+12];
                        gsp1[i*nd+7] = creal(coeff_c2s[227]) * gcart[15*i+ 2]
                                     - cimag(coeff_c2s[229]) * gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[232]) * gcart[15*i+ 7]
                                     - cimag(coeff_c2s[236]) * gcart[15*i+11]*_Complex_I;
                        gsp1[i*nd+8] = creal(coeff_c2s[255]) * gcart[15*i+ 0]
                                     - cimag(coeff_c2s[256]) * gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[258]) * gcart[15*i+ 3]
                                     - cimag(coeff_c2s[261]) * gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[265]) * gcart[15*i+10];
                        gsp1[i*nd+9] = 0;

                }
        }
}
static void g_bra_cart2spinor_e1sf(double complex *gsp, int nket,
                                   double *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        double complex *gsp1 = gsp + nket * nd;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[4].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = creal(coeff_c2s[0])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[1])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[3])*gcart[15*i+ 3]
                                     - cimag(coeff_c2s[6])*gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[10])*gcart[15*i+10];
                        gsp [i*nd+1] = creal(coeff_c2s[32])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[34])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[37])*gcart[15*i+ 7]
                                     - cimag(coeff_c2s[41])*gcart[15*i+11]*_Complex_I;
                        gsp [i*nd+2] = creal(coeff_c2s[60])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[61])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[65])*gcart[15*i+ 5]
                                     - cimag(coeff_c2s[66])*gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[68])*gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[70])*gcart[15*i+10]
                                     + creal(coeff_c2s[72])*gcart[15*i+12];
                        gsp [i*nd+3] = creal(coeff_c2s[92])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[94])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[97])*gcart[15*i+ 7]
                                     + creal(coeff_c2s[99])*gcart[15*i+ 9]
                                     - cimag(coeff_c2s[101])*gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[103])*gcart[15*i+13]*_Complex_I;
                        gsp [i*nd+4] = creal(coeff_c2s[120])*gcart[15*i+ 0]
                                     + creal(coeff_c2s[123])*gcart[15*i+ 3]
                                     + creal(coeff_c2s[125])*gcart[15*i+ 5]
                                     + creal(coeff_c2s[130])*gcart[15*i+10]
                                     + creal(coeff_c2s[132])*gcart[15*i+12]
                                     + creal(coeff_c2s[134])*gcart[15*i+14];
                        gsp [i*nd+5] = creal(coeff_c2s[152])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[154])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[157])*gcart[15*i+ 7]
                                     + creal(coeff_c2s[159])*gcart[15*i+ 9]
                                     - cimag(coeff_c2s[161])*gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[163])*gcart[15*i+13]*_Complex_I;
                        gsp [i*nd+6] = creal(coeff_c2s[180])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[181])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[185])*gcart[15*i+ 5]
                                     - cimag(coeff_c2s[186])*gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[188])*gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[190])*gcart[15*i+10]
                                     + creal(coeff_c2s[192])*gcart[15*i+12];
                        gsp [i*nd+7] = creal(coeff_c2s[212])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[214])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[217])*gcart[15*i+ 7]
                                     - cimag(coeff_c2s[221])*gcart[15*i+11]*_Complex_I;

                        gsp1[i*nd+0] = creal(coeff_c2s[17])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[19])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[22])*gcart[15*i+ 7]
                                     - cimag(coeff_c2s[26])*gcart[15*i+11]*_Complex_I;
                        gsp1[i*nd+1] = creal(coeff_c2s[45])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[46])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[50])*gcart[15*i+ 5]
                                     - cimag(coeff_c2s[51])*gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[53])*gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[55])*gcart[15*i+10]
                                     + creal(coeff_c2s[57])*gcart[15*i+12];
                        gsp1[i*nd+2] = creal(coeff_c2s[77])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[79])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[82])*gcart[15*i+ 7]
                                     + creal(coeff_c2s[84])*gcart[15*i+ 9]
                                     - cimag(coeff_c2s[86])*gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[88])*gcart[15*i+13]*_Complex_I;
                        gsp1[i*nd+3] = creal(coeff_c2s[105])*gcart[15*i+ 0]
                                     + creal(coeff_c2s[108])*gcart[15*i+ 3]
                                     + creal(coeff_c2s[110])*gcart[15*i+ 5]
                                     + creal(coeff_c2s[115])*gcart[15*i+10]
                                     + creal(coeff_c2s[117])*gcart[15*i+12]
                                     + creal(coeff_c2s[119])*gcart[15*i+14];
                        gsp1[i*nd+4] = creal(coeff_c2s[137])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[139])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[142])*gcart[15*i+ 7]
                                     + creal(coeff_c2s[144])*gcart[15*i+ 9]
                                     - cimag(coeff_c2s[146])*gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[148])*gcart[15*i+13]*_Complex_I;
                        gsp1[i*nd+5] = creal(coeff_c2s[165])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[166])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[170])*gcart[15*i+ 5]
                                     - cimag(coeff_c2s[171])*gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[173])*gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[175])*gcart[15*i+10]
                                     + creal(coeff_c2s[177])*gcart[15*i+12];
                        gsp1[i*nd+6] = creal(coeff_c2s[197])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[199])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[202])*gcart[15*i+ 7]
                                     - cimag(coeff_c2s[206])*gcart[15*i+11]*_Complex_I;
                        gsp1[i*nd+7] = creal(coeff_c2s[225])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[226])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[228])*gcart[15*i+ 3]
                                     - cimag(coeff_c2s[231])*gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[235])*gcart[15*i+10];
                }
                gsp  += 8;
                gsp1 += 8;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[4].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp [i*nd+0] = 0;
                        gsp [i*nd+1] = creal(coeff_c2s[30])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[31])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[33])*gcart[15*i+ 3]
                                     - cimag(coeff_c2s[36])*gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[40])*gcart[15*i+10];
                        gsp [i*nd+2] = creal(coeff_c2s[62])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[64])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[67])*gcart[15*i+ 7]
                                     - cimag(coeff_c2s[71])*gcart[15*i+11]*_Complex_I;
                        gsp [i*nd+3] = creal(coeff_c2s[90])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[91])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[95])*gcart[15*i+ 5]
                                     - cimag(coeff_c2s[96])*gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[98])*gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[100])*gcart[15*i+10]
                                     + creal(coeff_c2s[102])*gcart[15*i+12];
                        gsp [i*nd+4] = creal(coeff_c2s[122])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[124])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[127])*gcart[15*i+ 7]
                                     + creal(coeff_c2s[129])*gcart[15*i+ 9]
                                     - cimag(coeff_c2s[131])*gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[133])*gcart[15*i+13]*_Complex_I;
                        gsp [i*nd+5] = creal(coeff_c2s[150])*gcart[15*i+ 0]
                                     + creal(coeff_c2s[153])*gcart[15*i+ 3]
                                     + creal(coeff_c2s[155])*gcart[15*i+ 5]
                                     + creal(coeff_c2s[160])*gcart[15*i+10]
                                     + creal(coeff_c2s[162])*gcart[15*i+12]
                                     + creal(coeff_c2s[164])*gcart[15*i+14];
                        gsp [i*nd+6] = creal(coeff_c2s[182])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[184])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[187])*gcart[15*i+ 7]
                                     + creal(coeff_c2s[189])*gcart[15*i+ 9]
                                     - cimag(coeff_c2s[191])*gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[193])*gcart[15*i+13]*_Complex_I;
                        gsp [i*nd+7] = creal(coeff_c2s[210])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[211])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[215])*gcart[15*i+ 5]
                                     - cimag(coeff_c2s[216])*gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[218])*gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[220])*gcart[15*i+10]
                                     + creal(coeff_c2s[222])*gcart[15*i+12];
                        gsp [i*nd+8] = creal(coeff_c2s[242])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[244])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[247])*gcart[15*i+ 7]
                                     - cimag(coeff_c2s[251])*gcart[15*i+11]*_Complex_I;
                        gsp [i*nd+9] = creal(coeff_c2s[270])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[271])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[273])*gcart[15*i+ 3]
                                     - cimag(coeff_c2s[276])*gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[280])*gcart[15*i+10];

                        gsp1[i*nd+0] = creal(coeff_c2s[15])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[16])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[18])*gcart[15*i+ 3]
                                     - cimag(coeff_c2s[21])*gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[25])*gcart[15*i+10];
                        gsp1[i*nd+1] = creal(coeff_c2s[47])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[49])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[52])*gcart[15*i+ 7]
                                     - cimag(coeff_c2s[56])*gcart[15*i+11]*_Complex_I;
                        gsp1[i*nd+2] = creal(coeff_c2s[75])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[76])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[80])*gcart[15*i+ 5]
                                     - cimag(coeff_c2s[81])*gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[83])*gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[85])*gcart[15*i+10]
                                     + creal(coeff_c2s[87])*gcart[15*i+12];
                        gsp1[i*nd+3] = creal(coeff_c2s[107])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[109])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[112])*gcart[15*i+ 7]
                                     + creal(coeff_c2s[114])*gcart[15*i+ 9]
                                     - cimag(coeff_c2s[116])*gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[118])*gcart[15*i+13]*_Complex_I;
                        gsp1[i*nd+4] = creal(coeff_c2s[135])*gcart[15*i+ 0]
                                     + creal(coeff_c2s[138])*gcart[15*i+ 3]
                                     + creal(coeff_c2s[140])*gcart[15*i+ 5]
                                     + creal(coeff_c2s[145])*gcart[15*i+10]
                                     + creal(coeff_c2s[147])*gcart[15*i+12]
                                     + creal(coeff_c2s[149])*gcart[15*i+14];
                        gsp1[i*nd+5] = creal(coeff_c2s[167])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[169])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[172])*gcart[15*i+ 7]
                                     + creal(coeff_c2s[174])*gcart[15*i+ 9]
                                     - cimag(coeff_c2s[176])*gcart[15*i+11]*_Complex_I
                                     - cimag(coeff_c2s[178])*gcart[15*i+13]*_Complex_I;
                        gsp1[i*nd+6] = creal(coeff_c2s[195])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[196])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[200])*gcart[15*i+ 5]
                                     - cimag(coeff_c2s[201])*gcart[15*i+ 6]*_Complex_I
                                     - cimag(coeff_c2s[203])*gcart[15*i+ 8]*_Complex_I
                                     + creal(coeff_c2s[205])*gcart[15*i+10]
                                     + creal(coeff_c2s[207])*gcart[15*i+12];
                        gsp1[i*nd+7] = creal(coeff_c2s[227])*gcart[15*i+ 2]
                                     - cimag(coeff_c2s[229])*gcart[15*i+ 4]*_Complex_I
                                     + creal(coeff_c2s[232])*gcart[15*i+ 7]
                                     - cimag(coeff_c2s[236])*gcart[15*i+11]*_Complex_I;
                        gsp1[i*nd+8] = creal(coeff_c2s[255])*gcart[15*i+ 0]
                                     - cimag(coeff_c2s[256])*gcart[15*i+ 1]*_Complex_I
                                     + creal(coeff_c2s[258])*gcart[15*i+ 3]
                                     - cimag(coeff_c2s[261])*gcart[15*i+ 6]*_Complex_I
                                     + creal(coeff_c2s[265])*gcart[15*i+10];
                        gsp1[i*nd+9] = 0;

                }
        }
}
static void g_bra_cart2spinor_si(double complex *gsp, int nket,
                                 double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        const int nd = _len_spinor(l, kappa);
        double complex *gcart1 = gcart + nket * 15;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[4].cart2j_lt_l;
                for (i = 0; i < nket; i++) {
                        gsp[i*nd+0] = creal(coeff_c2s[0]) * gcart[15*i+ 0]
                                    - cimag(coeff_c2s[1]) * gcart[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[3]) * gcart[15*i+ 3]
                                    - cimag(coeff_c2s[6]) * gcart[15*i+ 6]*_Complex_I
                                    + creal(coeff_c2s[10]) * gcart[15*i+10];
                        gsp[i*nd+1] = creal(coeff_c2s[32]) * gcart[15*i+ 2]
                                    - cimag(coeff_c2s[34]) * gcart[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[37]) * gcart[15*i+ 7]
                                    - cimag(coeff_c2s[41]) * gcart[15*i+11]*_Complex_I;
                        gsp[i*nd+2] = creal(coeff_c2s[60]) * gcart[15*i+ 0]
                                    - cimag(coeff_c2s[61]) * gcart[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[65]) * gcart[15*i+ 5]
                                    - cimag(coeff_c2s[66]) * gcart[15*i+ 6]*_Complex_I
                                    - cimag(coeff_c2s[68]) * gcart[15*i+ 8]*_Complex_I
                                    + creal(coeff_c2s[70]) * gcart[15*i+10]
                                    + creal(coeff_c2s[72]) * gcart[15*i+12];
                        gsp[i*nd+3] = creal(coeff_c2s[92]) * gcart[15*i+ 2]
                                    - cimag(coeff_c2s[94]) * gcart[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[97]) * gcart[15*i+ 7]
                                    + creal(coeff_c2s[99]) * gcart[15*i+ 9]
                                    - cimag(coeff_c2s[101]) * gcart[15*i+11]*_Complex_I
                                    - cimag(coeff_c2s[103]) * gcart[15*i+13]*_Complex_I;
                        gsp[i*nd+4] = creal(coeff_c2s[120]) * gcart[15*i+ 0]
                                    + creal(coeff_c2s[123]) * gcart[15*i+ 3]
                                    + creal(coeff_c2s[125]) * gcart[15*i+ 5]
                                    + creal(coeff_c2s[130]) * gcart[15*i+10]
                                    + creal(coeff_c2s[132]) * gcart[15*i+12]
                                    + creal(coeff_c2s[134]) * gcart[15*i+14];
                        gsp[i*nd+5] = creal(coeff_c2s[152]) * gcart[15*i+ 2]
                                    - cimag(coeff_c2s[154]) * gcart[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[157]) * gcart[15*i+ 7]
                                    + creal(coeff_c2s[159]) * gcart[15*i+ 9]
                                    - cimag(coeff_c2s[161]) * gcart[15*i+11]*_Complex_I
                                    - cimag(coeff_c2s[163]) * gcart[15*i+13]*_Complex_I;
                        gsp[i*nd+6] = creal(coeff_c2s[180]) * gcart[15*i+ 0]
                                    - cimag(coeff_c2s[181]) * gcart[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[185]) * gcart[15*i+ 5]
                                    - cimag(coeff_c2s[186]) * gcart[15*i+ 6]*_Complex_I
                                    - cimag(coeff_c2s[188]) * gcart[15*i+ 8]*_Complex_I
                                    + creal(coeff_c2s[190]) * gcart[15*i+10]
                                    + creal(coeff_c2s[192]) * gcart[15*i+12];
                        gsp[i*nd+7] = creal(coeff_c2s[212]) * gcart[15*i+ 2]
                                    - cimag(coeff_c2s[214]) * gcart[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[217]) * gcart[15*i+ 7]
                                    - cimag(coeff_c2s[221]) * gcart[15*i+11]*_Complex_I;
                }
                for (i = 0; i < nket; i++) {
                        gsp[i*nd+0]+= creal(coeff_c2s[17]) * gcart1[15*i+ 2]
                                    - cimag(coeff_c2s[19]) * gcart1[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[22]) * gcart1[15*i+ 7]
                                    - cimag(coeff_c2s[26]) * gcart1[15*i+11]*_Complex_I;
                        gsp[i*nd+1]+= creal(coeff_c2s[45]) * gcart1[15*i+ 0]
                                    - cimag(coeff_c2s[46]) * gcart1[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[50]) * gcart1[15*i+ 5]
                                    - cimag(coeff_c2s[51]) * gcart1[15*i+ 6]*_Complex_I
                                    - cimag(coeff_c2s[53]) * gcart1[15*i+ 8]*_Complex_I
                                    + creal(coeff_c2s[55]) * gcart1[15*i+10]
                                    + creal(coeff_c2s[57]) * gcart1[15*i+12];
                        gsp[i*nd+2]+= creal(coeff_c2s[77]) * gcart1[15*i+ 2]
                                    - cimag(coeff_c2s[79]) * gcart1[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[82]) * gcart1[15*i+ 7]
                                    + creal(coeff_c2s[84]) * gcart1[15*i+ 9]
                                    - cimag(coeff_c2s[86]) * gcart1[15*i+11]*_Complex_I
                                    - cimag(coeff_c2s[88]) * gcart1[15*i+13]*_Complex_I;
                        gsp[i*nd+3]+= creal(coeff_c2s[105]) * gcart1[15*i+ 0]
                                    + creal(coeff_c2s[108]) * gcart1[15*i+ 3]
                                    + creal(coeff_c2s[110]) * gcart1[15*i+ 5]
                                    + creal(coeff_c2s[115]) * gcart1[15*i+10]
                                    + creal(coeff_c2s[117]) * gcart1[15*i+12]
                                    + creal(coeff_c2s[119]) * gcart1[15*i+14];
                        gsp[i*nd+4]+= creal(coeff_c2s[137]) * gcart1[15*i+ 2]
                                    - cimag(coeff_c2s[139]) * gcart1[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[142]) * gcart1[15*i+ 7]
                                    + creal(coeff_c2s[144]) * gcart1[15*i+ 9]
                                    - cimag(coeff_c2s[146]) * gcart1[15*i+11]*_Complex_I
                                    - cimag(coeff_c2s[148]) * gcart1[15*i+13]*_Complex_I;
                        gsp[i*nd+5]+= creal(coeff_c2s[165]) * gcart1[15*i+ 0]
                                    - cimag(coeff_c2s[166]) * gcart1[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[170]) * gcart1[15*i+ 5]
                                    - cimag(coeff_c2s[171]) * gcart1[15*i+ 6]*_Complex_I
                                    - cimag(coeff_c2s[173]) * gcart1[15*i+ 8]*_Complex_I
                                    + creal(coeff_c2s[175]) * gcart1[15*i+10]
                                    + creal(coeff_c2s[177]) * gcart1[15*i+12];
                        gsp[i*nd+6]+= creal(coeff_c2s[197]) * gcart1[15*i+ 2]
                                    - cimag(coeff_c2s[199]) * gcart1[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[202]) * gcart1[15*i+ 7]
                                    - cimag(coeff_c2s[206]) * gcart1[15*i+11]*_Complex_I;
                        gsp[i*nd+7]+= creal(coeff_c2s[225]) * gcart1[15*i+ 0]
                                    - cimag(coeff_c2s[226]) * gcart1[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[228]) * gcart1[15*i+ 3]
                                    - cimag(coeff_c2s[231]) * gcart1[15*i+ 6]*_Complex_I
                                    + creal(coeff_c2s[235]) * gcart1[15*i+10];
                }
                gsp += 8;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[4].cart2j_gt_l;
                for (i = 0; i < nket; i++) {
                        gsp[i*nd+0] = 0;
                        gsp[i*nd+1] = creal(coeff_c2s[30]) * gcart[15*i+ 0]
                                    - cimag(coeff_c2s[31]) * gcart[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[33]) * gcart[15*i+ 3]
                                    - cimag(coeff_c2s[36]) * gcart[15*i+ 6]*_Complex_I
                                    + creal(coeff_c2s[40]) * gcart[15*i+10];
                        gsp[i*nd+2] = creal(coeff_c2s[62]) * gcart[15*i+ 2]
                                    - cimag(coeff_c2s[64]) * gcart[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[67]) * gcart[15*i+ 7]
                                    - cimag(coeff_c2s[71]) * gcart[15*i+11]*_Complex_I;
                        gsp[i*nd+3] = creal(coeff_c2s[90]) * gcart[15*i+ 0]
                                    - cimag(coeff_c2s[91]) * gcart[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[95]) * gcart[15*i+ 5]
                                    - cimag(coeff_c2s[96]) * gcart[15*i+ 6]*_Complex_I
                                    - cimag(coeff_c2s[98]) * gcart[15*i+ 8]*_Complex_I
                                    + creal(coeff_c2s[100]) * gcart[15*i+10]
                                    + creal(coeff_c2s[102]) * gcart[15*i+12];
                        gsp[i*nd+4] = creal(coeff_c2s[122]) * gcart[15*i+ 2]
                                    - cimag(coeff_c2s[124]) * gcart[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[127]) * gcart[15*i+ 7]
                                    + creal(coeff_c2s[129]) * gcart[15*i+ 9]
                                    - cimag(coeff_c2s[131]) * gcart[15*i+11]*_Complex_I
                                    - cimag(coeff_c2s[133]) * gcart[15*i+13]*_Complex_I;
                        gsp[i*nd+5] = creal(coeff_c2s[150]) * gcart[15*i+ 0]
                                    + creal(coeff_c2s[153]) * gcart[15*i+ 3]
                                    + creal(coeff_c2s[155]) * gcart[15*i+ 5]
                                    + creal(coeff_c2s[160]) * gcart[15*i+10]
                                    + creal(coeff_c2s[162]) * gcart[15*i+12]
                                    + creal(coeff_c2s[164]) * gcart[15*i+14];
                        gsp[i*nd+6] = creal(coeff_c2s[182]) * gcart[15*i+ 2]
                                    - cimag(coeff_c2s[184]) * gcart[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[187]) * gcart[15*i+ 7]
                                    + creal(coeff_c2s[189]) * gcart[15*i+ 9]
                                    - cimag(coeff_c2s[191]) * gcart[15*i+11]*_Complex_I
                                    - cimag(coeff_c2s[193]) * gcart[15*i+13]*_Complex_I;
                        gsp[i*nd+7] = creal(coeff_c2s[210]) * gcart[15*i+ 0]
                                    - cimag(coeff_c2s[211]) * gcart[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[215]) * gcart[15*i+ 5]
                                    - cimag(coeff_c2s[216]) * gcart[15*i+ 6]*_Complex_I
                                    - cimag(coeff_c2s[218]) * gcart[15*i+ 8]*_Complex_I
                                    + creal(coeff_c2s[220]) * gcart[15*i+10]
                                    + creal(coeff_c2s[222]) * gcart[15*i+12];
                        gsp[i*nd+8] = creal(coeff_c2s[242]) * gcart[15*i+ 2]
                                    - cimag(coeff_c2s[244]) * gcart[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[247]) * gcart[15*i+ 7]
                                    - cimag(coeff_c2s[251]) * gcart[15*i+11]*_Complex_I;
                        gsp[i*nd+9] = creal(coeff_c2s[270]) * gcart[15*i+ 0]
                                    - cimag(coeff_c2s[271]) * gcart[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[273]) * gcart[15*i+ 3]
                                    - cimag(coeff_c2s[276]) * gcart[15*i+ 6]*_Complex_I
                                    + creal(coeff_c2s[280]) * gcart[15*i+10];
                }
                for (i = 0; i < nket; i++) {
                        gsp[i*nd+0]+= creal(coeff_c2s[15]) * gcart1[15*i+ 0]
                                    - cimag(coeff_c2s[16]) * gcart1[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[18]) * gcart1[15*i+ 3]
                                    - cimag(coeff_c2s[21]) * gcart1[15*i+ 6]*_Complex_I
                                    + creal(coeff_c2s[25]) * gcart1[15*i+10];
                        gsp[i*nd+1]+= creal(coeff_c2s[47]) * gcart1[15*i+ 2]
                                    - cimag(coeff_c2s[49]) * gcart1[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[52]) * gcart1[15*i+ 7]
                                    - cimag(coeff_c2s[56]) * gcart1[15*i+11]*_Complex_I;
                        gsp[i*nd+2]+= creal(coeff_c2s[75]) * gcart1[15*i+ 0]
                                    - cimag(coeff_c2s[76]) * gcart1[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[80]) * gcart1[15*i+ 5]
                                    - cimag(coeff_c2s[81]) * gcart1[15*i+ 6]*_Complex_I
                                    - cimag(coeff_c2s[83]) * gcart1[15*i+ 8]*_Complex_I
                                    + creal(coeff_c2s[85]) * gcart1[15*i+10]
                                    + creal(coeff_c2s[87]) * gcart1[15*i+12];
                        gsp[i*nd+3]+= creal(coeff_c2s[107]) * gcart1[15*i+ 2]
                                    - cimag(coeff_c2s[109]) * gcart1[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[112]) * gcart1[15*i+ 7]
                                    + creal(coeff_c2s[114]) * gcart1[15*i+ 9]
                                    - cimag(coeff_c2s[116]) * gcart1[15*i+11]*_Complex_I
                                    - cimag(coeff_c2s[118]) * gcart1[15*i+13]*_Complex_I;
                        gsp[i*nd+4]+= creal(coeff_c2s[135]) * gcart1[15*i+ 0]
                                    + creal(coeff_c2s[138]) * gcart1[15*i+ 3]
                                    + creal(coeff_c2s[140]) * gcart1[15*i+ 5]
                                    + creal(coeff_c2s[145]) * gcart1[15*i+10]
                                    + creal(coeff_c2s[147]) * gcart1[15*i+12]
                                    + creal(coeff_c2s[149]) * gcart1[15*i+14];
                        gsp[i*nd+5]+= creal(coeff_c2s[167]) * gcart1[15*i+ 2]
                                    - cimag(coeff_c2s[169]) * gcart1[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[172]) * gcart1[15*i+ 7]
                                    + creal(coeff_c2s[174]) * gcart1[15*i+ 9]
                                    - cimag(coeff_c2s[176]) * gcart1[15*i+11]*_Complex_I
                                    - cimag(coeff_c2s[178]) * gcart1[15*i+13]*_Complex_I;
                        gsp[i*nd+6]+= creal(coeff_c2s[195]) * gcart1[15*i+ 0]
                                    - cimag(coeff_c2s[196]) * gcart1[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[200]) * gcart1[15*i+ 5]
                                    - cimag(coeff_c2s[201]) * gcart1[15*i+ 6]*_Complex_I
                                    - cimag(coeff_c2s[203]) * gcart1[15*i+ 8]*_Complex_I
                                    + creal(coeff_c2s[205]) * gcart1[15*i+10]
                                    + creal(coeff_c2s[207]) * gcart1[15*i+12];
                        gsp[i*nd+7]+= creal(coeff_c2s[227]) * gcart1[15*i+ 2]
                                    - cimag(coeff_c2s[229]) * gcart1[15*i+ 4]*_Complex_I
                                    + creal(coeff_c2s[232]) * gcart1[15*i+ 7]
                                    - cimag(coeff_c2s[236]) * gcart1[15*i+11]*_Complex_I;
                        gsp[i*nd+8]+= creal(coeff_c2s[255]) * gcart1[15*i+ 0]
                                    - cimag(coeff_c2s[256]) * gcart1[15*i+ 1]*_Complex_I
                                    + creal(coeff_c2s[258]) * gcart1[15*i+ 3]
                                    - cimag(coeff_c2s[261]) * gcart1[15*i+ 6]*_Complex_I
                                    + creal(coeff_c2s[265]) * gcart1[15*i+10];
                        gsp[i*nd+9]+= 0;

                }
        }
}
static void g_ket_cart2spinor(double complex *gsp, int nbra,
                              double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[4].cart2j_lt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[0]) * gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[1]) * gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[3]) * gcart[nbra* 3+i]
                                      + cimag(coeff_c2s[6]) * gcart[nbra* 6+i]*_Complex_I
                                      + creal(coeff_c2s[10]) * gcart[nbra*10+i]
                                      + creal(coeff_c2s[17]) * gcart[nbra*17+i]
                                      + cimag(coeff_c2s[19]) * gcart[nbra*19+i]*_Complex_I
                                      + creal(coeff_c2s[22]) * gcart[nbra*22+i]
                                      + cimag(coeff_c2s[26]) * gcart[nbra*26+i]*_Complex_I;
                        gsp[1*nbra+i] = creal(coeff_c2s[32]) * gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[34]) * gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[37]) * gcart[nbra* 7+i]
                                      + cimag(coeff_c2s[41]) * gcart[nbra*11+i]*_Complex_I
                                      + creal(coeff_c2s[45]) * gcart[nbra*15+i]
                                      + cimag(coeff_c2s[46]) * gcart[nbra*16+i]*_Complex_I
                                      + creal(coeff_c2s[50]) * gcart[nbra*20+i]
                                      + cimag(coeff_c2s[51]) * gcart[nbra*21+i]*_Complex_I
                                      + cimag(coeff_c2s[53]) * gcart[nbra*23+i]*_Complex_I
                                      + creal(coeff_c2s[55]) * gcart[nbra*25+i]
                                      + creal(coeff_c2s[57]) * gcart[nbra*27+i];
                        gsp[2*nbra+i] = creal(coeff_c2s[60]) * gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[61]) * gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[65]) * gcart[nbra* 5+i]
                                      + cimag(coeff_c2s[66]) * gcart[nbra* 6+i]*_Complex_I
                                      + cimag(coeff_c2s[68]) * gcart[nbra* 8+i]*_Complex_I
                                      + creal(coeff_c2s[70]) * gcart[nbra*10+i]
                                      + creal(coeff_c2s[72]) * gcart[nbra*12+i]
                                      + creal(coeff_c2s[77]) * gcart[nbra*17+i]
                                      + cimag(coeff_c2s[79]) * gcart[nbra*19+i]*_Complex_I
                                      + creal(coeff_c2s[82]) * gcart[nbra*22+i]
                                      + creal(coeff_c2s[84]) * gcart[nbra*24+i]
                                      + cimag(coeff_c2s[86]) * gcart[nbra*26+i]*_Complex_I
                                      + cimag(coeff_c2s[88]) * gcart[nbra*28+i]*_Complex_I;
                        gsp[3*nbra+i] = creal(coeff_c2s[92]) * gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[94]) * gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[97]) * gcart[nbra* 7+i]
                                      + creal(coeff_c2s[99]) * gcart[nbra* 9+i]
                                      + cimag(coeff_c2s[101]) * gcart[nbra*11+i]*_Complex_I
                                      + cimag(coeff_c2s[103]) * gcart[nbra*13+i]*_Complex_I
                                      + creal(coeff_c2s[105]) * gcart[nbra*15+i]
                                      + creal(coeff_c2s[108]) * gcart[nbra*18+i]
                                      + creal(coeff_c2s[110]) * gcart[nbra*20+i]
                                      + creal(coeff_c2s[115]) * gcart[nbra*25+i]
                                      + creal(coeff_c2s[117]) * gcart[nbra*27+i]
                                      + creal(coeff_c2s[119]) * gcart[nbra*29+i];
                        gsp[4*nbra+i] = creal(coeff_c2s[120]) * gcart[nbra* 0+i]
                                      + creal(coeff_c2s[123]) * gcart[nbra* 3+i]
                                      + creal(coeff_c2s[125]) * gcart[nbra* 5+i]
                                      + creal(coeff_c2s[130]) * gcart[nbra*10+i]
                                      + creal(coeff_c2s[132]) * gcart[nbra*12+i]
                                      + creal(coeff_c2s[134]) * gcart[nbra*14+i]
                                      + creal(coeff_c2s[137]) * gcart[nbra*17+i]
                                      + cimag(coeff_c2s[139]) * gcart[nbra*19+i]*_Complex_I
                                      + creal(coeff_c2s[142]) * gcart[nbra*22+i]
                                      + creal(coeff_c2s[144]) * gcart[nbra*24+i]
                                      + cimag(coeff_c2s[146]) * gcart[nbra*26+i]*_Complex_I
                                      + cimag(coeff_c2s[148]) * gcart[nbra*28+i]*_Complex_I;
                        gsp[5*nbra+i] = creal(coeff_c2s[152]) * gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[154]) * gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[157]) * gcart[nbra* 7+i]
                                      + creal(coeff_c2s[159]) * gcart[nbra* 9+i]
                                      + cimag(coeff_c2s[161]) * gcart[nbra*11+i]*_Complex_I
                                      + cimag(coeff_c2s[163]) * gcart[nbra*13+i]*_Complex_I
                                      + creal(coeff_c2s[165]) * gcart[nbra*15+i]
                                      + cimag(coeff_c2s[166]) * gcart[nbra*16+i]*_Complex_I
                                      + creal(coeff_c2s[170]) * gcart[nbra*20+i]
                                      + cimag(coeff_c2s[171]) * gcart[nbra*21+i]*_Complex_I
                                      + cimag(coeff_c2s[173]) * gcart[nbra*23+i]*_Complex_I
                                      + creal(coeff_c2s[175]) * gcart[nbra*25+i]
                                      + creal(coeff_c2s[177]) * gcart[nbra*27+i];
                        gsp[6*nbra+i] = creal(coeff_c2s[180]) * gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[181]) * gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[185]) * gcart[nbra* 5+i]
                                      + cimag(coeff_c2s[186]) * gcart[nbra* 6+i]*_Complex_I
                                      + cimag(coeff_c2s[188]) * gcart[nbra* 8+i]*_Complex_I
                                      + creal(coeff_c2s[190]) * gcart[nbra*10+i]
                                      + creal(coeff_c2s[192]) * gcart[nbra*12+i]
                                      + creal(coeff_c2s[197]) * gcart[nbra*17+i]
                                      + cimag(coeff_c2s[199]) * gcart[nbra*19+i]*_Complex_I
                                      + creal(coeff_c2s[202]) * gcart[nbra*22+i]
                                      + cimag(coeff_c2s[206]) * gcart[nbra*26+i]*_Complex_I;
                        gsp[7*nbra+i] = creal(coeff_c2s[212]) * gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[214]) * gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[217]) * gcart[nbra* 7+i]
                                      + cimag(coeff_c2s[221]) * gcart[nbra*11+i]*_Complex_I
                                      + creal(coeff_c2s[225]) * gcart[nbra*15+i]
                                      + cimag(coeff_c2s[226]) * gcart[nbra*16+i]*_Complex_I
                                      + creal(coeff_c2s[228]) * gcart[nbra*18+i]
                                      + cimag(coeff_c2s[231]) * gcart[nbra*21+i]*_Complex_I
                                      + creal(coeff_c2s[235]) * gcart[nbra*25+i];
                };
                gsp += nbra * 8;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[4].cart2j_gt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[15]) * gcart[nbra*15+i]
                                      + cimag(coeff_c2s[16]) * gcart[nbra*16+i]*_Complex_I
                                      + creal(coeff_c2s[18]) * gcart[nbra*18+i]
                                      + cimag(coeff_c2s[21]) * gcart[nbra*21+i]*_Complex_I
                                      + creal(coeff_c2s[25]) * gcart[nbra*25+i];
                        gsp[1*nbra+i] = creal(coeff_c2s[30]) * gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[31]) * gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[33]) * gcart[nbra* 3+i]
                                      + cimag(coeff_c2s[36]) * gcart[nbra* 6+i]*_Complex_I
                                      + creal(coeff_c2s[40]) * gcart[nbra*10+i]
                                      + creal(coeff_c2s[47]) * gcart[nbra*17+i]
                                      + cimag(coeff_c2s[49]) * gcart[nbra*19+i]*_Complex_I
                                      + creal(coeff_c2s[52]) * gcart[nbra*22+i]
                                      + cimag(coeff_c2s[56]) * gcart[nbra*26+i]*_Complex_I;
                        gsp[2*nbra+i] = creal(coeff_c2s[62]) * gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[64]) * gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[67]) * gcart[nbra* 7+i]
                                      + cimag(coeff_c2s[71]) * gcart[nbra*11+i]*_Complex_I
                                      + creal(coeff_c2s[75]) * gcart[nbra*15+i]
                                      + cimag(coeff_c2s[76]) * gcart[nbra*16+i]*_Complex_I
                                      + creal(coeff_c2s[80]) * gcart[nbra*20+i]
                                      + cimag(coeff_c2s[81]) * gcart[nbra*21+i]*_Complex_I
                                      + cimag(coeff_c2s[83]) * gcart[nbra*23+i]*_Complex_I
                                      + creal(coeff_c2s[85]) * gcart[nbra*25+i]
                                      + creal(coeff_c2s[87]) * gcart[nbra*27+i];
                        gsp[3*nbra+i] = creal(coeff_c2s[90]) * gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[91]) * gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[95]) * gcart[nbra* 5+i]
                                      + cimag(coeff_c2s[96]) * gcart[nbra* 6+i]*_Complex_I
                                      + cimag(coeff_c2s[98]) * gcart[nbra* 8+i]*_Complex_I
                                      + creal(coeff_c2s[100]) * gcart[nbra*10+i]
                                      + creal(coeff_c2s[102]) * gcart[nbra*12+i]
                                      + creal(coeff_c2s[107]) * gcart[nbra*17+i]
                                      + cimag(coeff_c2s[109]) * gcart[nbra*19+i]*_Complex_I
                                      + creal(coeff_c2s[112]) * gcart[nbra*22+i]
                                      + creal(coeff_c2s[114]) * gcart[nbra*24+i]
                                      + cimag(coeff_c2s[116]) * gcart[nbra*26+i]*_Complex_I
                                      + cimag(coeff_c2s[118]) * gcart[nbra*28+i]*_Complex_I;
                        gsp[4*nbra+i] = creal(coeff_c2s[122]) * gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[124]) * gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[127]) * gcart[nbra* 7+i]
                                      + creal(coeff_c2s[129]) * gcart[nbra* 9+i]
                                      + cimag(coeff_c2s[131]) * gcart[nbra*11+i]*_Complex_I
                                      + cimag(coeff_c2s[133]) * gcart[nbra*13+i]*_Complex_I
                                      + creal(coeff_c2s[135]) * gcart[nbra*15+i]
                                      + creal(coeff_c2s[138]) * gcart[nbra*18+i]
                                      + creal(coeff_c2s[140]) * gcart[nbra*20+i]
                                      + creal(coeff_c2s[145]) * gcart[nbra*25+i]
                                      + creal(coeff_c2s[147]) * gcart[nbra*27+i]
                                      + creal(coeff_c2s[149]) * gcart[nbra*29+i];
                        gsp[5*nbra+i] = creal(coeff_c2s[150]) * gcart[nbra* 0+i]
                                      + creal(coeff_c2s[153]) * gcart[nbra* 3+i]
                                      + creal(coeff_c2s[155]) * gcart[nbra* 5+i]
                                      + creal(coeff_c2s[160]) * gcart[nbra*10+i]
                                      + creal(coeff_c2s[162]) * gcart[nbra*12+i]
                                      + creal(coeff_c2s[164]) * gcart[nbra*14+i]
                                      + creal(coeff_c2s[167]) * gcart[nbra*17+i]
                                      + cimag(coeff_c2s[169]) * gcart[nbra*19+i]*_Complex_I
                                      + creal(coeff_c2s[172]) * gcart[nbra*22+i]
                                      + creal(coeff_c2s[174]) * gcart[nbra*24+i]
                                      + cimag(coeff_c2s[176]) * gcart[nbra*26+i]*_Complex_I
                                      + cimag(coeff_c2s[178]) * gcart[nbra*28+i]*_Complex_I;
                        gsp[6*nbra+i] = creal(coeff_c2s[182]) * gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[184]) * gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[187]) * gcart[nbra* 7+i]
                                      + creal(coeff_c2s[189]) * gcart[nbra* 9+i]
                                      + cimag(coeff_c2s[191]) * gcart[nbra*11+i]*_Complex_I
                                      + cimag(coeff_c2s[193]) * gcart[nbra*13+i]*_Complex_I
                                      + creal(coeff_c2s[195]) * gcart[nbra*15+i]
                                      + cimag(coeff_c2s[196]) * gcart[nbra*16+i]*_Complex_I
                                      + creal(coeff_c2s[200]) * gcart[nbra*20+i]
                                      + cimag(coeff_c2s[201]) * gcart[nbra*21+i]*_Complex_I
                                      + cimag(coeff_c2s[203]) * gcart[nbra*23+i]*_Complex_I
                                      + creal(coeff_c2s[205]) * gcart[nbra*25+i]
                                      + creal(coeff_c2s[207]) * gcart[nbra*27+i];
                        gsp[7*nbra+i] = creal(coeff_c2s[210]) * gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[211]) * gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[215]) * gcart[nbra* 5+i]
                                      + cimag(coeff_c2s[216]) * gcart[nbra* 6+i]*_Complex_I
                                      + cimag(coeff_c2s[218]) * gcart[nbra* 8+i]*_Complex_I
                                      + creal(coeff_c2s[220]) * gcart[nbra*10+i]
                                      + creal(coeff_c2s[222]) * gcart[nbra*12+i]
                                      + creal(coeff_c2s[227]) * gcart[nbra*17+i]
                                      + cimag(coeff_c2s[229]) * gcart[nbra*19+i]*_Complex_I
                                      + creal(coeff_c2s[232]) * gcart[nbra*22+i]
                                      + cimag(coeff_c2s[236]) * gcart[nbra*26+i]*_Complex_I;
                        gsp[8*nbra+i] = creal(coeff_c2s[242]) * gcart[nbra* 2+i]
                                      + cimag(coeff_c2s[244]) * gcart[nbra* 4+i]*_Complex_I
                                      + creal(coeff_c2s[247]) * gcart[nbra* 7+i]
                                      + cimag(coeff_c2s[251]) * gcart[nbra*11+i]*_Complex_I
                                      + creal(coeff_c2s[255]) * gcart[nbra*15+i]
                                      + cimag(coeff_c2s[256]) * gcart[nbra*16+i]*_Complex_I
                                      + creal(coeff_c2s[258]) * gcart[nbra*18+i]
                                      + cimag(coeff_c2s[261]) * gcart[nbra*21+i]*_Complex_I
                                      + creal(coeff_c2s[265]) * gcart[nbra*25+i];
                        gsp[9*nbra+i] = creal(coeff_c2s[270]) * gcart[nbra* 0+i]
                                      + cimag(coeff_c2s[271]) * gcart[nbra* 1+i]*_Complex_I
                                      + creal(coeff_c2s[273]) * gcart[nbra* 3+i]
                                      + cimag(coeff_c2s[276]) * gcart[nbra* 6+i]*_Complex_I
                                      + creal(coeff_c2s[280]) * gcart[nbra*10+i];
                }
        }
}

static void g_iket_cart2spinor(double complex *gsp, int nbra,
                               double complex *gcart, int l, int kappa)
{
        const double complex *coeff_c2s;
        int i;

        if (kappa >= 0) {
                coeff_c2s = g_c2s[4].cart2j_lt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[0]) * gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[1]) * gcart[nbra* 1+i]
                                      + creal(coeff_c2s[3]) * gcart[nbra* 3+i]*_Complex_I
                                      - cimag(coeff_c2s[6]) * gcart[nbra* 6+i]
                                      + creal(coeff_c2s[10]) * gcart[nbra*10+i]*_Complex_I
                                      + creal(coeff_c2s[17]) * gcart[nbra*17+i]*_Complex_I
                                      - cimag(coeff_c2s[19]) * gcart[nbra*19+i]
                                      + creal(coeff_c2s[22]) * gcart[nbra*22+i]*_Complex_I
                                      - cimag(coeff_c2s[26]) * gcart[nbra*26+i];
                        gsp[1*nbra+i] = creal(coeff_c2s[32]) * gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[34]) * gcart[nbra* 4+i]
                                      + creal(coeff_c2s[37]) * gcart[nbra* 7+i]*_Complex_I
                                      - cimag(coeff_c2s[41]) * gcart[nbra*11+i]
                                      + creal(coeff_c2s[45]) * gcart[nbra*15+i]*_Complex_I
                                      - cimag(coeff_c2s[46]) * gcart[nbra*16+i]
                                      + creal(coeff_c2s[50]) * gcart[nbra*20+i]*_Complex_I
                                      - cimag(coeff_c2s[51]) * gcart[nbra*21+i]
                                      - cimag(coeff_c2s[53]) * gcart[nbra*23+i]
                                      + creal(coeff_c2s[55]) * gcart[nbra*25+i]*_Complex_I
                                      + creal(coeff_c2s[57]) * gcart[nbra*27+i]*_Complex_I;
                        gsp[2*nbra+i] = creal(coeff_c2s[60]) * gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[61]) * gcart[nbra* 1+i]
                                      + creal(coeff_c2s[65]) * gcart[nbra* 5+i]*_Complex_I
                                      - cimag(coeff_c2s[66]) * gcart[nbra* 6+i]
                                      - cimag(coeff_c2s[68]) * gcart[nbra* 8+i]
                                      + creal(coeff_c2s[70]) * gcart[nbra*10+i]*_Complex_I
                                      + creal(coeff_c2s[72]) * gcart[nbra*12+i]*_Complex_I
                                      + creal(coeff_c2s[77]) * gcart[nbra*17+i]*_Complex_I
                                      - cimag(coeff_c2s[79]) * gcart[nbra*19+i]
                                      + creal(coeff_c2s[82]) * gcart[nbra*22+i]*_Complex_I
                                      + creal(coeff_c2s[84]) * gcart[nbra*24+i]*_Complex_I
                                      - cimag(coeff_c2s[86]) * gcart[nbra*26+i]
                                      - cimag(coeff_c2s[88]) * gcart[nbra*28+i];
                        gsp[3*nbra+i] = creal(coeff_c2s[92]) * gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[94]) * gcart[nbra* 4+i]
                                      + creal(coeff_c2s[97]) * gcart[nbra* 7+i]*_Complex_I
                                      + creal(coeff_c2s[99]) * gcart[nbra* 9+i]*_Complex_I
                                      - cimag(coeff_c2s[101]) * gcart[nbra*11+i]
                                      - cimag(coeff_c2s[103]) * gcart[nbra*13+i]
                                      + creal(coeff_c2s[105]) * gcart[nbra*15+i]*_Complex_I
                                      + creal(coeff_c2s[108]) * gcart[nbra*18+i]*_Complex_I
                                      + creal(coeff_c2s[110]) * gcart[nbra*20+i]*_Complex_I
                                      + creal(coeff_c2s[115]) * gcart[nbra*25+i]*_Complex_I
                                      + creal(coeff_c2s[117]) * gcart[nbra*27+i]*_Complex_I
                                      + creal(coeff_c2s[119]) * gcart[nbra*29+i]*_Complex_I;
                        gsp[4*nbra+i] = creal(coeff_c2s[120]) * gcart[nbra* 0+i]*_Complex_I
                                      + creal(coeff_c2s[123]) * gcart[nbra* 3+i]*_Complex_I
                                      + creal(coeff_c2s[125]) * gcart[nbra* 5+i]*_Complex_I
                                      + creal(coeff_c2s[130]) * gcart[nbra*10+i]*_Complex_I
                                      + creal(coeff_c2s[132]) * gcart[nbra*12+i]*_Complex_I
                                      + creal(coeff_c2s[134]) * gcart[nbra*14+i]*_Complex_I
                                      + creal(coeff_c2s[137]) * gcart[nbra*17+i]*_Complex_I
                                      - cimag(coeff_c2s[139]) * gcart[nbra*19+i]
                                      + creal(coeff_c2s[142]) * gcart[nbra*22+i]*_Complex_I
                                      + creal(coeff_c2s[144]) * gcart[nbra*24+i]*_Complex_I
                                      - cimag(coeff_c2s[146]) * gcart[nbra*26+i]
                                      - cimag(coeff_c2s[148]) * gcart[nbra*28+i];
                        gsp[5*nbra+i] = creal(coeff_c2s[152]) * gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[154]) * gcart[nbra* 4+i]
                                      + creal(coeff_c2s[157]) * gcart[nbra* 7+i]*_Complex_I
                                      + creal(coeff_c2s[159]) * gcart[nbra* 9+i]*_Complex_I
                                      - cimag(coeff_c2s[161]) * gcart[nbra*11+i]
                                      - cimag(coeff_c2s[163]) * gcart[nbra*13+i]
                                      + creal(coeff_c2s[165]) * gcart[nbra*15+i]*_Complex_I
                                      - cimag(coeff_c2s[166]) * gcart[nbra*16+i]
                                      + creal(coeff_c2s[170]) * gcart[nbra*20+i]*_Complex_I
                                      - cimag(coeff_c2s[171]) * gcart[nbra*21+i]
                                      - cimag(coeff_c2s[173]) * gcart[nbra*23+i]
                                      + creal(coeff_c2s[175]) * gcart[nbra*25+i]*_Complex_I
                                      + creal(coeff_c2s[177]) * gcart[nbra*27+i]*_Complex_I;
                        gsp[6*nbra+i] = creal(coeff_c2s[180]) * gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[181]) * gcart[nbra* 1+i]
                                      + creal(coeff_c2s[185]) * gcart[nbra* 5+i]*_Complex_I
                                      - cimag(coeff_c2s[186]) * gcart[nbra* 6+i]
                                      - cimag(coeff_c2s[188]) * gcart[nbra* 8+i]
                                      + creal(coeff_c2s[190]) * gcart[nbra*10+i]*_Complex_I
                                      + creal(coeff_c2s[192]) * gcart[nbra*12+i]*_Complex_I
                                      + creal(coeff_c2s[197]) * gcart[nbra*17+i]*_Complex_I
                                      - cimag(coeff_c2s[199]) * gcart[nbra*19+i]
                                      + creal(coeff_c2s[202]) * gcart[nbra*22+i]*_Complex_I
                                      - cimag(coeff_c2s[206]) * gcart[nbra*26+i];
                        gsp[7*nbra+i] = creal(coeff_c2s[212]) * gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[214]) * gcart[nbra* 4+i]
                                      + creal(coeff_c2s[217]) * gcart[nbra* 7+i]*_Complex_I
                                      - cimag(coeff_c2s[221]) * gcart[nbra*11+i]
                                      + creal(coeff_c2s[225]) * gcart[nbra*15+i]*_Complex_I
                                      - cimag(coeff_c2s[226]) * gcart[nbra*16+i]
                                      + creal(coeff_c2s[228]) * gcart[nbra*18+i]*_Complex_I
                                      - cimag(coeff_c2s[231]) * gcart[nbra*21+i]
                                      + creal(coeff_c2s[235]) * gcart[nbra*25+i]*_Complex_I;
                };
                gsp += nbra * 8;
        }
        if (kappa <= 0) {
                coeff_c2s = g_c2s[4].cart2j_gt_l;
                for (i = 0; i < nbra; i++) {
                        gsp[0*nbra+i] = creal(coeff_c2s[15]) * gcart[nbra*15+i]*_Complex_I
                                      - cimag(coeff_c2s[16]) * gcart[nbra*16+i]
                                      + creal(coeff_c2s[18]) * gcart[nbra*18+i]*_Complex_I
                                      - cimag(coeff_c2s[21]) * gcart[nbra*21+i]
                                      + creal(coeff_c2s[25]) * gcart[nbra*25+i]*_Complex_I;
                        gsp[1*nbra+i] = creal(coeff_c2s[30]) * gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[31]) * gcart[nbra* 1+i]
                                      + creal(coeff_c2s[33]) * gcart[nbra* 3+i]*_Complex_I
                                      - cimag(coeff_c2s[36]) * gcart[nbra* 6+i]
                                      + creal(coeff_c2s[40]) * gcart[nbra*10+i]*_Complex_I
                                      + creal(coeff_c2s[47]) * gcart[nbra*17+i]*_Complex_I
                                      - cimag(coeff_c2s[49]) * gcart[nbra*19+i]
                                      + creal(coeff_c2s[52]) * gcart[nbra*22+i]*_Complex_I
                                      - cimag(coeff_c2s[56]) * gcart[nbra*26+i];
                        gsp[2*nbra+i] = creal(coeff_c2s[62]) * gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[64]) * gcart[nbra* 4+i]
                                      + creal(coeff_c2s[67]) * gcart[nbra* 7+i]*_Complex_I
                                      - cimag(coeff_c2s[71]) * gcart[nbra*11+i]
                                      + creal(coeff_c2s[75]) * gcart[nbra*15+i]*_Complex_I
                                      - cimag(coeff_c2s[76]) * gcart[nbra*16+i]
                                      + creal(coeff_c2s[80]) * gcart[nbra*20+i]*_Complex_I
                                      - cimag(coeff_c2s[81]) * gcart[nbra*21+i]
                                      - cimag(coeff_c2s[83]) * gcart[nbra*23+i]
                                      + creal(coeff_c2s[85]) * gcart[nbra*25+i]*_Complex_I
                                      + creal(coeff_c2s[87]) * gcart[nbra*27+i]*_Complex_I;
                        gsp[3*nbra+i] = creal(coeff_c2s[90]) * gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[91]) * gcart[nbra* 1+i]
                                      + creal(coeff_c2s[95]) * gcart[nbra* 5+i]*_Complex_I
                                      - cimag(coeff_c2s[96]) * gcart[nbra* 6+i]
                                      - cimag(coeff_c2s[98]) * gcart[nbra* 8+i]
                                      + creal(coeff_c2s[100]) * gcart[nbra*10+i]*_Complex_I
                                      + creal(coeff_c2s[102]) * gcart[nbra*12+i]*_Complex_I
                                      + creal(coeff_c2s[107]) * gcart[nbra*17+i]*_Complex_I
                                      - cimag(coeff_c2s[109]) * gcart[nbra*19+i]
                                      + creal(coeff_c2s[112]) * gcart[nbra*22+i]*_Complex_I
                                      + creal(coeff_c2s[114]) * gcart[nbra*24+i]*_Complex_I
                                      - cimag(coeff_c2s[116]) * gcart[nbra*26+i]
                                      - cimag(coeff_c2s[118]) * gcart[nbra*28+i];
                        gsp[4*nbra+i] = creal(coeff_c2s[122]) * gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[124]) * gcart[nbra* 4+i]
                                      + creal(coeff_c2s[127]) * gcart[nbra* 7+i]*_Complex_I
                                      + creal(coeff_c2s[129]) * gcart[nbra* 9+i]*_Complex_I
                                      - cimag(coeff_c2s[131]) * gcart[nbra*11+i]
                                      - cimag(coeff_c2s[133]) * gcart[nbra*13+i]
                                      + creal(coeff_c2s[135]) * gcart[nbra*15+i]*_Complex_I
                                      + creal(coeff_c2s[138]) * gcart[nbra*18+i]*_Complex_I
                                      + creal(coeff_c2s[140]) * gcart[nbra*20+i]*_Complex_I
                                      + creal(coeff_c2s[145]) * gcart[nbra*25+i]*_Complex_I
                                      + creal(coeff_c2s[147]) * gcart[nbra*27+i]*_Complex_I
                                      + creal(coeff_c2s[149]) * gcart[nbra*29+i]*_Complex_I;
                        gsp[5*nbra+i] = creal(coeff_c2s[150]) * gcart[nbra* 0+i]*_Complex_I
                                      + creal(coeff_c2s[153]) * gcart[nbra* 3+i]*_Complex_I
                                      + creal(coeff_c2s[155]) * gcart[nbra* 5+i]*_Complex_I
                                      + creal(coeff_c2s[160]) * gcart[nbra*10+i]*_Complex_I
                                      + creal(coeff_c2s[162]) * gcart[nbra*12+i]*_Complex_I
                                      + creal(coeff_c2s[164]) * gcart[nbra*14+i]*_Complex_I
                                      + creal(coeff_c2s[167]) * gcart[nbra*17+i]*_Complex_I
                                      - cimag(coeff_c2s[169]) * gcart[nbra*19+i]
                                      + creal(coeff_c2s[172]) * gcart[nbra*22+i]*_Complex_I
                                      + creal(coeff_c2s[174]) * gcart[nbra*24+i]*_Complex_I
                                      - cimag(coeff_c2s[176]) * gcart[nbra*26+i]
                                      - cimag(coeff_c2s[178]) * gcart[nbra*28+i];
                        gsp[6*nbra+i] = creal(coeff_c2s[182]) * gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[184]) * gcart[nbra* 4+i]
                                      + creal(coeff_c2s[187]) * gcart[nbra* 7+i]*_Complex_I
                                      + creal(coeff_c2s[189]) * gcart[nbra* 9+i]*_Complex_I
                                      - cimag(coeff_c2s[191]) * gcart[nbra*11+i]
                                      - cimag(coeff_c2s[193]) * gcart[nbra*13+i]
                                      + creal(coeff_c2s[195]) * gcart[nbra*15+i]*_Complex_I
                                      - cimag(coeff_c2s[196]) * gcart[nbra*16+i]
                                      + creal(coeff_c2s[200]) * gcart[nbra*20+i]*_Complex_I
                                      - cimag(coeff_c2s[201]) * gcart[nbra*21+i]
                                      - cimag(coeff_c2s[203]) * gcart[nbra*23+i]
                                      + creal(coeff_c2s[205]) * gcart[nbra*25+i]*_Complex_I
                                      + creal(coeff_c2s[207]) * gcart[nbra*27+i]*_Complex_I;
                        gsp[7*nbra+i] = creal(coeff_c2s[210]) * gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[211]) * gcart[nbra* 1+i]
                                      + creal(coeff_c2s[215]) * gcart[nbra* 5+i]*_Complex_I
                                      - cimag(coeff_c2s[216]) * gcart[nbra* 6+i]
                                      - cimag(coeff_c2s[218]) * gcart[nbra* 8+i]
                                      + creal(coeff_c2s[220]) * gcart[nbra*10+i]*_Complex_I
                                      + creal(coeff_c2s[222]) * gcart[nbra*12+i]*_Complex_I
                                      + creal(coeff_c2s[227]) * gcart[nbra*17+i]*_Complex_I
                                      - cimag(coeff_c2s[229]) * gcart[nbra*19+i]
                                      + creal(coeff_c2s[232]) * gcart[nbra*22+i]*_Complex_I
                                      - cimag(coeff_c2s[236]) * gcart[nbra*26+i];
                        gsp[8*nbra+i] = creal(coeff_c2s[242]) * gcart[nbra* 2+i]*_Complex_I
                                      - cimag(coeff_c2s[244]) * gcart[nbra* 4+i]
                                      + creal(coeff_c2s[247]) * gcart[nbra* 7+i]*_Complex_I
                                      - cimag(coeff_c2s[251]) * gcart[nbra*11+i]
                                      + creal(coeff_c2s[255]) * gcart[nbra*15+i]*_Complex_I
                                      - cimag(coeff_c2s[256]) * gcart[nbra*16+i]
                                      + creal(coeff_c2s[258]) * gcart[nbra*18+i]*_Complex_I
                                      - cimag(coeff_c2s[261]) * gcart[nbra*21+i]
                                      + creal(coeff_c2s[265]) * gcart[nbra*25+i]*_Complex_I;
                        gsp[9*nbra+i] = creal(coeff_c2s[270]) * gcart[nbra* 0+i]*_Complex_I
                                      - cimag(coeff_c2s[271]) * gcart[nbra* 1+i]
                                      + creal(coeff_c2s[273]) * gcart[nbra* 3+i]*_Complex_I
                                      - cimag(coeff_c2s[276]) * gcart[nbra* 6+i]
                                      + creal(coeff_c2s[280]) * gcart[nbra*10+i]*_Complex_I;
                }
        }
}

static void (*f_bra_spinor_e1sf[6])() = {
        s_bra_cart2spinor_e1sf,
        p_bra_cart2spinor_e1sf,
        d_bra_cart2spinor_e1sf,
        f_bra_cart2spinor_e1sf,
        g_bra_cart2spinor_e1sf,
        a_bra_cart2spinor_e1sf,
};

static void (*f_bra_spinor_sf[6])() = {
        s_bra_cart2spinor_sf,
        p_bra_cart2spinor_sf,
        d_bra_cart2spinor_sf,
        f_bra_cart2spinor_sf,
        g_bra_cart2spinor_sf,
        a_bra_cart2spinor_sf,
};

static void (*f_ket_spinor[6])() = {
        s_ket_cart2spinor,
        p_ket_cart2spinor,
        d_ket_cart2spinor,
        f_ket_cart2spinor,
        g_ket_cart2spinor,
        a_ket_cart2spinor,
};

static void (*f_iket_spinor[6])() = {
        s_iket_cart2spinor,
        p_iket_cart2spinor,
        d_iket_cart2spinor,
        f_iket_cart2spinor,
        g_iket_cart2spinor,
        a_iket_cart2spinor,
};

static void (*f_bra_spinor_si[6])() = {
        s_bra_cart2spinor_si,
        p_bra_cart2spinor_si,
        d_bra_cart2spinor_si,
        f_bra_cart2spinor_si,
        g_bra_cart2spinor_si,
        a_bra_cart2spinor_si,
};

/*************************************************
 *
 * transform matrices
 *
 *************************************************/

/*
 * (i,k,l,j) -> (k,i,j,l)
 */
static void dswap_ik_jl(double *new, const double *old,
                        const int ni, const int nj, const int nk, const int nl)
{
        int j, l;
        int dlo = ni * nk; // shift of (i,k,l++,j)
        int djo = ni * nk * nl; // shift of (i,k,l,j++)
        int djn = nk * ni; // shift of (k,i,j++,l)
        int dln = nk * ni * nj; // shift of (k,i,j,l++)

        for (l = 0; l < nl; l++)
                for (j = 0; j < nj; j++) {
                        CINTdmat_transpose(new + l * dln + j * djn,
                                           old + l * dlo + j * djo, ni, nk);
                }
}

/*
 * (i,k,l,j) -> (k,i,j,l)
 */
static void zswap_ik_jl(double complex *new, const double complex *old,
                        const int ni, const int nj, const int nk, const int nl)
{
        int j, l;
        int dlo = ni * nk; // shift of (i,k,l++,j)
        int djo = ni * nk * nl; // shift of (i,k,l,j++)
        int djn = nk * ni; // shift of (k,i,j++,l)
        int dln = nk * ni * nj; // shift of (k,i,j,l++)

        for (l = 0; l < nl; l++)
                for (j = 0; j < nj; j++) {
                        CINTzmat_transpose(new + l * dln + j * djn,
                                           old + l * dlo + j * djo, ni, nk);
                }
}


static void dcopy_ij(double *opij, const double *gctr, 
                     const int ni, const int nj, const int mi, const int mj)
{
        int i, j;

        for (j = 0; j < mj; j++) {
                for (i = 0; i < mi; i++) {
                        opij[j*ni+i] = gctr[j*mi+i];
                }
        }
}
static void zcopy_ij(double complex *opij, const double complex *gctr, 
                     const int ni, const int nj, const int mi, const int mj)
{
        int i, j;

        for (j = 0; j < mj; j++) {
                for (i = 0; i < mi; i++) {
                        opij[j*ni+i] = gctr[j*mi+i];
                }
        }
}

/*
 * fijkl(ic:ic-1+di,jc:jc-1+dj,kc:kc-1+dk,lc:lc-1+dl)
 * fijkl(ni,nj,nk,nl), gctr(mk,mi,mj,ml)
 * gctr(k,i,j,l) -> fijkl(i,j,k,l)
 */
static void dcopy_kijl(double *fijkl, const double *gctr, 
                       const int ni, const int nj, const int nk, const int nl,
                       const int mi, const int mj, const int mk, const int ml)
{
        int i, j, k, l;
        double *pl, *pk, *pk1, *pk2, *pj;
        const double *pgctr, *pgctr1, *pgctr2;

        for (l = 0; l < ml; l++) {
                pl = fijkl + l * nk * ni * nj;
                switch (mk) {
                case 1:
                        pgctr = gctr + l * mi * mj;
                        switch (mi) {
                        case 1:
                                for (j = 0; j < mj; j++) {
                                        pl[j*ni] = pgctr[j];
                                }
                                break;
                        case 3:
                                for (j = 0; j < mj; j++) {
                                        pj = pl + j * ni;
                                        pj[0] = pgctr[0];
                                        pj[1] = pgctr[1];
                                        pj[2] = pgctr[2];
                                        pgctr += 3;
                                }
                                break;
                        case 5:
                                for (j = 0; j < mj; j++) {
                                        pj = pl + j * ni;
                                        pj[0] = pgctr[0];
                                        pj[1] = pgctr[1];
                                        pj[2] = pgctr[2];
                                        pj[3] = pgctr[3];
                                        pj[4] = pgctr[4];
                                        pgctr += 5;
                                }
                                break;
                        default:
                                for (j = 0; j < mj; j++) {
                                        pj = pl + j * ni;
                                        for (i = 0; i < mi; i++) {
                                                pj[i] = pgctr[i];
                                        }
                                        pgctr += mi;
                                }
                        }
                        break;
                case 3:
                        pk  = pl + 0 * ni * nj;
                        pk1 = pl + 1 * ni * nj;
                        pk2 = pl + 2 * ni * nj;
                        pgctr  = gctr + l * 3 * mi * mj + 0;
                        pgctr1 = gctr + l * 3 * mi * mj + 1;
                        pgctr2 = gctr + l * 3 * mi * mj + 2;
                        switch (mi) {
                        case 1:
                                for (j = 0; j < mj; j++) {
                                        pk [j*ni] = pgctr [j*3];
                                        pk1[j*ni] = pgctr1[j*3];
                                        pk2[j*ni] = pgctr2[j*3];
                                }
                                break;
                        case 3:
                                for (j = 0; j < mj; j++) {
                                        pk [j*ni+0] = pgctr [0*3];
                                        pk [j*ni+1] = pgctr [1*3];
                                        pk [j*ni+2] = pgctr [2*3];
                                        pk1[j*ni+0] = pgctr1[0*3];
                                        pk1[j*ni+1] = pgctr1[1*3];
                                        pk1[j*ni+2] = pgctr1[2*3];
                                        pk2[j*ni+0] = pgctr2[0*3];
                                        pk2[j*ni+1] = pgctr2[1*3];
                                        pk2[j*ni+2] = pgctr2[2*3];
                                        pgctr  += 3*3;
                                        pgctr1 += 3*3;
                                        pgctr2 += 3*3;
                                }
                                break;
                        case 5:
                                for (j = 0; j < mj; j++) {
                                        pk [j*ni+0] = pgctr [0*3];
                                        pk [j*ni+1] = pgctr [1*3];
                                        pk [j*ni+2] = pgctr [2*3];
                                        pk [j*ni+3] = pgctr [3*3];
                                        pk [j*ni+4] = pgctr [4*3];
                                        pk1[j*ni+0] = pgctr1[0*3];
                                        pk1[j*ni+1] = pgctr1[1*3];
                                        pk1[j*ni+2] = pgctr1[2*3];
                                        pk1[j*ni+3] = pgctr1[3*3];
                                        pk1[j*ni+4] = pgctr1[4*3];
                                        pk2[j*ni+0] = pgctr2[0*3];
                                        pk2[j*ni+1] = pgctr2[1*3];
                                        pk2[j*ni+2] = pgctr2[2*3];
                                        pk2[j*ni+3] = pgctr2[3*3];
                                        pk2[j*ni+4] = pgctr2[4*3];
                                        pgctr  += 5*3;
                                        pgctr1 += 5*3;
                                        pgctr2 += 5*3;
                                }
                                break;
                        default:
                                for (j = 0; j < mj; j++) {
                                        for (i = 0; i < mi; i++) {
                                                pk [j*ni+i] = pgctr [3*i];
                                                pk1[j*ni+i] = pgctr1[3*i];
                                                pk2[j*ni+i] = pgctr2[3*i];
                                        }
                                        pgctr  += 3 * mi;
                                        pgctr1 += 3 * mi;
                                        pgctr2 += 3 * mi;
                                }
                        }
                        break;
                default:
                        for (k = 0; k < mk; k++) {
                                pk = pl + k * ni * nj;
                                pgctr = gctr + l * mk * mi * mj + k;
                                //for (j = 0; j < mj; j++) {
                                //        pj = pk + j * ni;
                                //        for (i = 0; i < mi; i++) {
                                //                pj[i] = *pgctr;
                                //                pgctr += mk;
                                //        }
                                //}
                                switch (mi) {
                                case 1:
                                        for (j = 0; j < mj; j++) {
                                                pk[j*ni] = pgctr[j*mk];
                                        }
                                        break;
                                case 3:
                                        for (j = 0; j < mj; j++) {
                                                pj = pk + j * ni;
                                                pj[0] = pgctr[0*mk];
                                                pj[1] = pgctr[1*mk];
                                                pj[2] = pgctr[2*mk];
                                                pgctr += mk*3;
                                        }
                                        break;
                                case 5:
                                        for (j = 0; j < mj; j++) {
                                                pj = pk + j * ni;
                                                pj[0] = pgctr[0*mk];
                                                pj[1] = pgctr[1*mk];
                                                pj[2] = pgctr[2*mk];
                                                pj[3] = pgctr[3*mk];
                                                pj[4] = pgctr[4*mk];
                                                pgctr += mk*5;
                                        }
                                        break;
                                default:
                                        for (j = 0; j < mj; j++) {
                                                pj = pk + j * ni;
                                                for (i = 0; i < mi; i++) {
                                                        pj[i] = pgctr[mk*i];
                                                }
                                                pgctr += mk*mi;
                                        }
                                }
                        }
                }
        }
}

static void zcopy_kijl(double complex *fijkl, const double complex *gctr,
                       const int ni, const int nj, const int nk, const int nl,
                       const int mi, const int mj, const int mk, const int ml)
{
        int i, j, k, l;
        double complex *pl, *pk, *pj;
        const double complex *pgctr;

        for (l = 0; l < ml; l++) {
                pl = fijkl + l * nk * ni * nj;
                for (k = 0; k < mk; k++) {
                        pk = pl + k * ni * nj;
                        pgctr = gctr + (l * mk * mi * mj + k);
                        for (j = 0; j < mj; j++) {
                                pj = pk + j * ni;
                                for (i = 0; i < mi; i++) {
                                        pj[i] = pgctr[i*mk];
                                }
                                pgctr += mk * mi;
                        }
                }
        }
}

/*
 * gctr(i,k,l,j) -> fijkl(i,j,k,l)
static void dcopy_iklj(double *fijkl, const double *gctr, 
                       const int ni, const int nj, const int nk, const int nl,
                       const int mi, const int mj, const int mk, const int ml)
{
        int i, j, k, l;
        double *pl, *pk, *pj;
        const double *pgctr;

        for (l = 0; l < ml; l++) {
                pl = fijkl + l * nk * ni * nj;
                for (k = 0; k < mk; k++) {
                        pk = pl + k * ni * nj;
                        pgctr = gctr + l * mi * mk + k * mi;
                        for (j = 0; j < mj; j++) {
                                pj = pk + j * ni;
                                for (i = 0; i < mi; i++) {
                                        pj[i] = pgctr[i];
                                }
                                pgctr += mi * mk * ml;
                        }
                }
        }
} */


/*
 * 1e integrals, cartesian to real spheric.
 */
void c2s_sph_1e(double *opij, const double *gctr,
                const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int di = i_l * 2 + 1;
        const int dj = j_l * 2 + 1;
        const int ni = di * i_ctr;
        const int nj = dj * j_ctr;
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nf = nfi * nfj;
        int ic, jc;
        double *const buf1 = (double *)malloc(sizeof(double) * di*nfj*2);
        double *const buf2 = buf1 + di * nfj;
        double *tmp1;

        for (jc = 0; jc < nj; jc += dj)
                for (ic = 0; ic < ni; ic += di) {
                        tmp1 = (f_bra_sph[i_l])(buf1, nfj, gctr, i_l);
                        tmp1 = (f_ket_sph[j_l])(buf2, di, tmp1, j_l);
                        dcopy_ij(opij+ni*jc+ic, tmp1, ni, nj, di, dj);
                        gctr += nf;
                }
        free(buf1);
}


/*
 * 1e integrals, cartesian to spin free spinor.
 */
void c2s_sf_1e(double complex *opij, const double *gctr,
               const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int ni = di * i_ctr;
        const int nj = dj * j_ctr;
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nf2j = nfj + nfj;
        const int nf = nfi * nfj;
        int ic, jc;
        double complex *const tmp1 = malloc(sizeof(double complex) * di*nf2j * 2);
        double complex *const tmp2 = tmp1 + di*nf2j;

        for (jc = 0; jc < nj; jc += dj)
                for (ic = 0; ic < ni; ic += di) {
                        (f_bra_spinor_e1sf[i_l])(tmp1, nfj, gctr, i_l, i_kp);
                        (f_ket_spinor[j_l])(tmp2, di, tmp1, j_l, j_kp);
                        zcopy_ij(opij+ni*jc+ic, tmp2, ni, nj, di, dj);
                        gctr += nf;
                }

        free(tmp1);
}
void c2s_sf_1ei(double complex *opij, const double *gctr,
                const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int ni = di * i_ctr;
        const int nj = dj * j_ctr;
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nf2j = nfj + nfj;
        const int nf = nfi * nfj;
        int ic, jc;
        double complex *const tmp1 = malloc(sizeof(double complex) * di*nf2j * 2);
        double complex *const tmp2 = tmp1 + di*nf2j;

        for (jc = 0; jc < nj; jc += dj)
                for (ic = 0; ic < ni; ic += di) {
                        (f_bra_spinor_e1sf[i_l])(tmp1, nfj, gctr, i_l, i_kp);
                        (f_iket_spinor[j_l])(tmp2, di, tmp1, j_l, j_kp);
                        zcopy_ij(opij+ni*jc+ic, tmp2, ni, nj, di, dj);
                        gctr += nf;
                }

        free(tmp1);
}


/*
 * 1e integrals, cartesian to spinor.
 */
void c2s_si_1e(double complex *opij, const double *gctr,
               const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int ni = di * i_ctr;
        const int nj = dj * j_ctr;
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nf2i = nfi + nfi;
        const int nf2j = nfj + nfj;
        const int nf = nfi * nfj;
        int ic, jc;
        const double *gc_x = gctr;
        const double *gc_y = gc_x + nf * i_ctr * j_ctr;
        const double *gc_z = gc_y + nf * i_ctr * j_ctr;
        const double *gc_1 = gc_z + nf * i_ctr * j_ctr;
        double complex *const tmp1 = malloc(sizeof(double complex)*nf2i*nf2j
                                            +sizeof(double complex)*di*nf2j);
        double complex *const tmp2 = tmp1 + nf2i*nf2j;

        for (jc = 0; jc < nj; jc += dj)
                for (ic = 0; ic < ni; ic += di) {
                        //cmplx( gctr.POS_1, gctr.POS_Z)
                        //cmplx( gctr.POS_Y, gctr.POS_X)
                        CINTdcmplx_pp(nf, tmp1, gc_1, gc_z);
                        CINTdcmplx_pp(nf, tmp1+nf, gc_y, gc_x);
                        //cmplx(-gctr.POS_Y, gctr.POS_X)
                        //cmplx( gctr.POS_1,-gctr.POS_Z)
                        CINTdcmplx_np(nf, tmp1+nfi*nf2j, gc_y, gc_x);
                        CINTdcmplx_pn(nf, tmp1+nfi*nf2j+nf, gc_1, gc_z);
                        (f_bra_spinor_si[i_l])(tmp2, nf2j, tmp1, i_l, i_kp);
                        (f_ket_spinor[j_l])(tmp1, di, tmp2, j_l, j_kp);
                        zcopy_ij(opij+ni*jc+ic, tmp1, ni, nj, di, dj);

                        gc_x += nf;
                        gc_y += nf;
                        gc_z += nf;
                        gc_1 += nf;
                }
        free(tmp1);
}
void c2s_si_1ei(double complex *opij, const double *gctr,
                const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int ni = di * i_ctr;
        const int nj = dj * j_ctr;
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nf2i = nfi + nfi;
        const int nf2j = nfj + nfj;
        const int nf = nfi * nfj;
        int ic, jc;
        const double *gc_x = gctr;
        const double *gc_y = gc_x + nf * i_ctr * j_ctr;
        const double *gc_z = gc_y + nf * i_ctr * j_ctr;
        const double *gc_1 = gc_z + nf * i_ctr * j_ctr;
        double complex *const tmp1 = malloc(sizeof(double complex)*nf2i*nf2j
                                            +sizeof(double complex)*di*nf2j);
        double complex *const tmp2 = tmp1 + nf2i*nf2j;

        for (jc = 0; jc < nj; jc += dj)
                for (ic = 0; ic < ni; ic += di) {
                        //cmplx( gctr.POS_1, gctr.POS_Z)
                        //cmplx( gctr.POS_Y, gctr.POS_X)
                        CINTdcmplx_pp(nf, tmp1, gc_1, gc_z);
                        CINTdcmplx_pp(nf, tmp1+nf, gc_y, gc_x);
                        //cmplx(-gctr.POS_Y, gctr.POS_X)
                        //cmplx( gctr.POS_1,-gctr.POS_Z)
                        CINTdcmplx_np(nf, tmp1+nfi*nf2j, gc_y, gc_x);
                        CINTdcmplx_pn(nf, tmp1+nfi*nf2j+nf, gc_1, gc_z);
                        (f_bra_spinor_si[i_l])(tmp2, nf2j, tmp1, i_l, i_kp);
                        (f_iket_spinor[j_l])(tmp1, di, tmp2, j_l, j_kp);
                        zcopy_ij(opij+ni*jc+ic, tmp1, ni, nj, di, dj);

                        gc_x += nf;
                        gc_y += nf;
                        gc_z += nf;
                        gc_1 += nf;
                }
        free(tmp1);
}


/*
 * 2e integrals, cartesian to real spheric.
 *
 * gctr: Cartesian GTO integrals, ordered as <ik|lj>
 */
void c2s_sph_2e1(double *fijkl, const double *gctr,
                 const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int k_sh = shls[2];
        const int l_sh = shls[3];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int k_l = bas(ANG_OF, k_sh);
        const int l_l = bas(ANG_OF, l_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int k_ctr = bas(NCTR_OF, k_sh);
        const int l_ctr = bas(NCTR_OF, l_sh);
        const int di = i_l * 2 + 1;
        const int dj = j_l * 2 + 1;
        const int dk = k_l * 2 + 1;
        const int dl = l_l * 2 + 1;
        const int ni = di * i_ctr;
        const int nj = dj * j_ctr;
        const int nk = dk * k_ctr;
        const int nl = dl * l_ctr;
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nfk = CINTlen_cart(k_l);
        const int nfl = CINTlen_cart(l_l);
        const int nf = nfi * nfk * nfl * nfj;
        const int d_i = di * nfk * nfl;
        const int d_j = nfk * nfl * nfj;
        const int d_k = dk * di * dj;
        const int d_l = di * dj * nfl;
        int ofj = ni;
        int ofk = ni * nj;
        int ofl = nk * ni * nj;
        int ic, jc, kc, lc;
        double *pfijkl;
        const int buflen = (di*nfk*nfl*nfj + 16) & 0xfffffff0; // align for SSE
        double *const buf1 = (double *)malloc(sizeof(double)*buflen*5);
        double *const buf2 = buf1 + buflen;
        double *const buf3 = buf2 + buflen;
        double *const buf4 = buf3 + buflen;
        double *const buf5 = buf4 + buflen;
        double *tmp1;

        for (lc = 0; lc < nl; lc += dl) {
                for (kc = 0; kc < nk; kc += dk) {
                        for (jc = 0; jc < nj; jc += dj) {
                                for (ic = 0; ic < ni; ic += di) {
        tmp1 = (f_bra_sph[i_l])(buf1, d_j, gctr, i_l);
        tmp1 = (f_ket_sph[j_l])(buf2, d_i, tmp1, j_l);
        gctr += nf;

        dswap_ik_jl(buf3, tmp1, di, dj, nfk, nfl);
        tmp1 = (f_bra_sph[k_l])(buf4, d_l, buf3, k_l);
        tmp1 = (f_ket_sph[l_l])(buf5, d_k, tmp1, l_l);
        pfijkl = fijkl + ofl * lc + ofk * kc + ofj * jc + ic;

        dcopy_kijl(pfijkl, tmp1, ni, nj, nk, nl, di, dj, dk, dl);
                                } } } }

        free(buf1);
}
void c2s_sph_2e2() {};


/*
 * 2e integrals, cartesian to spin free spinor for electron 1.
 *
 * gctr: Cartesian GTO integrals, ordered as <ik|lj>
 * opij: partial transformed GTO integrals, ordered as <ik|lj>
 */
void c2s_sf_2e1(double complex *opij, const double *gctr,
                const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int k_sh = shls[2];
        const int l_sh = shls[3];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int k_l = bas(ANG_OF, k_sh);
        const int l_l = bas(ANG_OF, l_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int k_ctr = bas(NCTR_OF, k_sh);
        const int l_ctr = bas(NCTR_OF, l_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nfk = CINTlen_cart(k_l);
        const int nfl = CINTlen_cart(l_l);
        const int nf2j = nfj + nfj;
        const int nf = nfi * nfk * nfl * nfj;
        const int no = di * nfk * nfl * dj;
        const int d_i = di * nfk * nfl;
        const int d_j = nfk * nfl * nfj;
        int i;
        double complex *const tmp1 = malloc(sizeof(double complex)
                                            * di*nfk*nfl*nf2j);

        for (i = 0; i < i_ctr * j_ctr * k_ctr * l_ctr; i++) {
                (f_bra_spinor_e1sf[i_l])(tmp1, d_j, gctr, i_l, i_kp);
                (f_ket_spinor[j_l])(opij, d_i, tmp1, j_l, j_kp);
                gctr += nf;
                opij += no;
        }

        free(tmp1);
}
void c2s_sf_2e1i(double complex *opij, const double *gctr,
                 const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int k_sh = shls[2];
        const int l_sh = shls[3];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int k_l = bas(ANG_OF, k_sh);
        const int l_l = bas(ANG_OF, l_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int k_ctr = bas(NCTR_OF, k_sh);
        const int l_ctr = bas(NCTR_OF, l_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nfk = CINTlen_cart(k_l);
        const int nfl = CINTlen_cart(l_l);
        const int nf2j = nfj + nfj;
        const int nf = nfi * nfk * nfl * nfj;
        const int no = di * nfk * nfl * dj;
        const int d_i = di * nfk * nfl;
        const int d_j = nfk * nfl * nfj;
        int i;
        double  complex*const tmp1 = malloc(sizeof(double complex)
                                            * di*nfk*nfl*nf2j);

        for (i = 0; i < i_ctr * j_ctr * k_ctr * l_ctr; i++) {
                (f_bra_spinor_e1sf[i_l])(tmp1, d_j, gctr, i_l, i_kp);
                (f_iket_spinor[j_l])(opij, d_i, tmp1, j_l, j_kp);
                gctr += nf;
                opij += no;
        }

        free(tmp1);
}


/*
 * 2e integrals, cartesian to spin free spinor for electron 2.
 *
 * opij: partial transformed GTO integrals, ordered as <ik|lj>
 */
void c2s_sf_2e2(double complex *fijkl, const double complex *opij,
                const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int k_sh = shls[2];
        const int l_sh = shls[3];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int k_l = bas(ANG_OF, k_sh);
        const int l_l = bas(ANG_OF, l_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int k_kp = bas(KAPPA_OF, k_sh);
        const int l_kp = bas(KAPPA_OF, l_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int k_ctr = bas(NCTR_OF, k_sh);
        const int l_ctr = bas(NCTR_OF, l_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int dk = _len_spinor(k_l, k_kp);
        const int dl = _len_spinor(l_l, l_kp);
        const int ni = di * i_ctr;
        const int nj = dj * j_ctr;
        const int nk = dk * k_ctr;
        const int nl = dl * l_ctr;
        const int nfk = CINTlen_cart(k_l);
        const int nfl = CINTlen_cart(l_l);
        const int nf2k = nfk + nfk;
        const int nf2l = nfl + nfl;
        const int d_k = dk * di * dj;
        const int d_l = di * dj * nfl;
        const int nop = nfk * di * dj * nfl;
        int ofj = ni;
        int ofk = ni * nj;
        int ofl = nk * ni * nj;
        int ic, jc, kc, lc;
        double complex *pfijkl;
        const int len1 = (nf2k*di*dj*nf2l + 16) & 0xfffffff0;
        const int len2 = dk*di*dj*nf2l;
        double complex *const tmp1 = malloc(sizeof(double complex)
                                            * (len1+len2));
        double complex *const tmp2 = tmp1 + len1;

        for (lc = 0; lc < nl; lc += dl) {
                for (kc = 0; kc < nk; kc += dk) {
                        for (jc = 0; jc < nj; jc += dj) {
                                for (ic = 0; ic < ni; ic += di) {
        zswap_ik_jl(tmp1, opij, di, dj, nfk, nfl);
        (f_bra_spinor_sf[k_l])(tmp2, d_l, tmp1, k_l, k_kp);
        (f_ket_spinor[l_l])(tmp1, d_k, tmp2, l_l, l_kp);
        pfijkl = fijkl + (ofl * lc + ofk * kc + ofj * jc + ic);

        zcopy_kijl(pfijkl, tmp1, ni, nj, nk, nl, di, dj, dk, dl);
        opij += nop;
                                } } } }

        free(tmp1);
}
void c2s_sf_2e2i(double complex *fijkl, const double complex *opij,
                 const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int k_sh = shls[2];
        const int l_sh = shls[3];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int k_l = bas(ANG_OF, k_sh);
        const int l_l = bas(ANG_OF, l_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int k_kp = bas(KAPPA_OF, k_sh);
        const int l_kp = bas(KAPPA_OF, l_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int k_ctr = bas(NCTR_OF, k_sh);
        const int l_ctr = bas(NCTR_OF, l_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int dk = _len_spinor(k_l, k_kp);
        const int dl = _len_spinor(l_l, l_kp);
        const int ni = di * i_ctr;
        const int nj = dj * j_ctr;
        const int nk = dk * k_ctr;
        const int nl = dl * l_ctr;
        const int nfk = CINTlen_cart(k_l);
        const int nfl = CINTlen_cart(l_l);
        const int nf2k = nfk + nfk;
        const int nf2l = nfl + nfl;
        const int d_k = dk * di * dj;
        const int d_l = di * dj * nfl;
        const int nop = nfk * di * dj * nfl;
        int ofj = ni;
        int ofk = ni * nj;
        int ofl = nk * ni * nj;
        int ic, jc, kc, lc;
        double complex *pfijkl;
        const int len1 = (nf2k*di*dj*nf2l + 16) & 0xfffffff0;
        const int len2 = dk*di*dj*nf2l;
        double complex *const tmp1 = malloc(sizeof(double complex)
                                            * (len1+len2));
        double complex *const tmp2 = tmp1 + len1;

        for (lc = 0; lc < nl; lc += dl) {
                for (kc = 0; kc < nk; kc += dk) {
                        for (jc = 0; jc < nj; jc += dj) {
                                for (ic = 0; ic < ni; ic += di) {
        zswap_ik_jl(tmp1, opij, di, dj, nfk, nfl);
        (f_bra_spinor_sf[k_l])(tmp2, d_l, tmp1, k_l, k_kp);
        (f_iket_spinor[l_l])(tmp1, d_k, tmp2, l_l, l_kp);
        pfijkl = fijkl + (ofl * lc + ofk * kc + ofj * jc + ic);

        zcopy_kijl(pfijkl, tmp1, ni, nj, nk, nl, di, dj, dk, dl);
        opij += nop;
                                } } } }

        free(tmp1);
}

/*
 * 2e integrals, cartesian to spinor for electron 1.
 *
 * gctr: Cartesian GTO integrals, ordered as <ik|lj>
 * opij: partial transformed GTO integrals, ordered as <ik|lj>
 */
void c2s_si_2e1(double complex *opij, const double *gctr,
                const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int k_sh = shls[2];
        const int l_sh = shls[3];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int k_l = bas(ANG_OF, k_sh);
        const int l_l = bas(ANG_OF, l_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int k_ctr = bas(NCTR_OF, k_sh);
        const int l_ctr = bas(NCTR_OF, l_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nfk = CINTlen_cart(k_l);
        const int nfl = CINTlen_cart(l_l);
        const int nf2i = nfi + nfi;
        const int nf2j = nfj + nfj;
        const int nf = nfi * nfk * nfl * nfj;
        const int no = di * nfk * nfl * dj;
        const int d_i = di * nfk * nfl;
        const int d_j = nfk * nfl * nf2j;
        int i;
        const double *gc_x = gctr;
        const double *gc_y = gc_x + nf * i_ctr * j_ctr * k_ctr * l_ctr;
        const double *gc_z = gc_y + nf * i_ctr * j_ctr * k_ctr * l_ctr;
        const double *gc_1 = gc_z + nf * i_ctr * j_ctr * k_ctr * l_ctr;
        const int len1 = (nf2i*nfk*nfl*nf2j + 16) & 0xfffffff0;
        const int len2 = di*nfk*nfl*nf2j;
        double complex *const tmp1 = malloc(sizeof(double complex)
                                            * (len1+len2));
        double complex *const tmp2 = tmp1 + len1;

        for (i = 0; i < i_ctr * j_ctr * k_ctr * l_ctr; i++) {
                //cmplx( gctr.POS_1, gctr.POS_Z)
                //cmplx( gctr.POS_Y, gctr.POS_X)
                CINTdcmplx_pp(nf, tmp1, gc_1, gc_z);
                CINTdcmplx_pp(nf, tmp1+nf, gc_y, gc_x);
                //cmplx(-gctr.POS_Y, gctr.POS_X)
                //cmplx( gctr.POS_1,-gctr.POS_Z)
                CINTdcmplx_np(nf, tmp1+nfi*d_j, gc_y, gc_x);
                CINTdcmplx_pn(nf, tmp1+nfi*d_j+nf, gc_1, gc_z);
                (f_bra_spinor_si[i_l])(tmp2, d_j, tmp1, i_l, i_kp);
                (f_ket_spinor[j_l])(opij, d_i, tmp2, j_l, j_kp);
                gc_x += nf;
                gc_y += nf;
                gc_z += nf;
                gc_1 += nf;
                opij += no;
        }

        free(tmp1);
}
void c2s_si_2e1i(double complex *opij, const double *gctr,
                 const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int k_sh = shls[2];
        const int l_sh = shls[3];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int k_l = bas(ANG_OF, k_sh);
        const int l_l = bas(ANG_OF, l_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int k_ctr = bas(NCTR_OF, k_sh);
        const int l_ctr = bas(NCTR_OF, l_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nfk = CINTlen_cart(k_l);
        const int nfl = CINTlen_cart(l_l);
        const int nf2i = nfi + nfi;
        const int nf2j = nfj + nfj;
        const int nf = nfi * nfk * nfl * nfj;
        const int no = di * nfk * nfl * dj;
        const int d_i = di * nfk * nfl;
        const int d_j = nfk * nfl * nf2j;
        int i;
        const double *gc_x = gctr;
        const double *gc_y = gc_x + nf * i_ctr * j_ctr * k_ctr * l_ctr;
        const double *gc_z = gc_y + nf * i_ctr * j_ctr * k_ctr * l_ctr;
        const double *gc_1 = gc_z + nf * i_ctr * j_ctr * k_ctr * l_ctr;
        const int len1 = (nf2i*nfk*nfl*nf2j + 16) & 0xfffffff0;
        const int len2 = di*nfk*nfl*nf2j;
        double complex *const tmp1 = malloc(sizeof(double complex)
                                            * (len1+len2));
        double complex *const tmp2 = tmp1 + len1;

        for (i = 0; i < i_ctr * j_ctr * k_ctr * l_ctr; i++) {
                //cmplx( gctr.POS_1, gctr.POS_Z)
                //cmplx( gctr.POS_Y, gctr.POS_X)
                CINTdcmplx_pp(nf, tmp1, gc_1, gc_z);
                CINTdcmplx_pp(nf, tmp1+nf, gc_y, gc_x);
                //cmplx(-gctr.POS_Y, gctr.POS_X)
                //cmplx( gctr.POS_1,-gctr.POS_Z)
                CINTdcmplx_np(nf, tmp1+nfi*d_j, gc_y, gc_x);
                CINTdcmplx_pn(nf, tmp1+nfi*d_j+nf, gc_1, gc_z);
                (f_bra_spinor_si[i_l])(tmp2, d_j, tmp1, i_l, i_kp);
                (f_iket_spinor[j_l])(opij, d_i, tmp2, j_l, j_kp);
                gc_x += nf;
                gc_y += nf;
                gc_z += nf;
                gc_1 += nf;
                opij += no;
        }
        free(tmp1);
}

/*
 * 2e integrals, cartesian to spinor for electron 2.
 *
 * opij: partial transformed GTO integrals, ordered as <ik|lj>
 */
static void si2e_swap(double complex *new,
                      const double complex *oldx, const double complex *oldy,
                      const double complex *oldz, const double complex *old1,
                      const int ni, const int nj, const int nk, const int nl)
{
        int i, j, k, l;
        int dlo = ni * nk; // shift of (i,k,l++,j)
        int djo = ni * nk * nl; // shift of (i,k,l,j++)
        int djn = nk * ni; // shift of (k,i,j++,l)
        int dln = nk * ni * nj; // shift of (k,i,j,l++)
        double complex *new11 = new;
        double complex *new12 = new11 + nk * ni * nj * nl;
        double complex *new21 = new12 + nk * ni * nj * nl;
        double complex *new22 = new21 + nk * ni * nj * nl;
        double complex *pn11, *pn12, *pn21, *pn22;
        const double complex *ox, *oy, *oz, *o1;
        const double complex *pox, *poy, *poz, *po1;

        //tmp1(k    ,i,j,l    ) = opij(m,n,POS_1) + IZ1*opij(m,n,POS_Z)
        //tmp1(k    ,i,j,l+nfl) = opij(m,n,POS_Y) + IZ1*opij(m,n,POS_X)
        //tmp1(k+nfk,i,j,l    ) =-opij(m,n,POS_Y) + IZ1*opij(m,n,POS_X)
        //tmp1(k+nfk,i,j,l+nfl) = opij(m,n,POS_1) - IZ1*opij(m,n,POS_Z)
        for (l = 0; l < nl; l++)
                for (j = 0; j < nj; j++) {
                        pn11 = new11 + l * dln + j * djn;
                        pn12 = new12 + l * dln + j * djn;
                        pn21 = new21 + l * dln + j * djn;
                        pn22 = new22 + l * dln + j * djn;
                        ox = oldx + l * dlo + j * djo;
                        oy = oldy + l * dlo + j * djo;
                        oz = oldz + l * dlo + j * djo;
                        o1 = old1 + l * dlo + j * djo;
                        for (i = 0; i < ni; i++) {
                                pox = ox + i;
                                poy = oy + i;
                                poz = oz + i;
                                po1 = o1 + i;
                                for (k = 0; k < nk; k++) {
                                        pn11[k] = po1[0] + poz[0]*_Complex_I;
                                        pn12[k] = poy[0] + pox[0]*_Complex_I;
                                        pn21[k] =-poy[0] + pox[0]*_Complex_I;
                                        pn22[k] = po1[0] - poz[0]*_Complex_I;
                                        pox += ni;
                                        poy += ni;
                                        poz += ni;
                                        po1 += ni;
                                }
                                pn11 += nk;
                                pn12 += nk;
                                pn21 += nk;
                                pn22 += nk;
                        }
                }
}
void c2s_si_2e2(double complex *fijkl, const double complex *opij,
                const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int k_sh = shls[2];
        const int l_sh = shls[3];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int k_l = bas(ANG_OF, k_sh);
        const int l_l = bas(ANG_OF, l_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int k_kp = bas(KAPPA_OF, k_sh);
        const int l_kp = bas(KAPPA_OF, l_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int k_ctr = bas(NCTR_OF, k_sh);
        const int l_ctr = bas(NCTR_OF, l_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int dk = _len_spinor(k_l, k_kp);
        const int dl = _len_spinor(l_l, l_kp);
        const int ni = di * i_ctr;
        const int nj = dj * j_ctr;
        const int nk = dk * k_ctr;
        const int nl = dl * l_ctr;
        const int nfk = CINTlen_cart(k_l);
        const int nfl = CINTlen_cart(l_l);
        const int nf2k = nfk + nfk;
        const int nf2l = nfl + nfl;
        const int d_k = dk * di * dj;
        const int d_l = di * dj * nf2l;
        const int nop = nfk * di * dj * nfl;
        int ofj = ni;
        int ofk = ni * nj;
        int ofl = nk * ni * nj;
        int ic, jc, kc, lc;
        double complex *pfijkl;
        const double complex *ox = opij;
        const double complex *oy = ox + nop * i_ctr * j_ctr * k_ctr * l_ctr;
        const double complex *oz = oy + nop * i_ctr * j_ctr * k_ctr * l_ctr;
        const double complex *o1 = oz + nop * i_ctr * j_ctr * k_ctr * l_ctr;
        const int len1 = (nf2k*di*dj*nf2l + 16) & 0xfffffff0;
        const int len2 = dk*di*dj*nf2l;
        double complex *const tmp1 = malloc(sizeof(double complex)
                                            * (len1+len2));
        double complex *const tmp2 = tmp1 + len1;

        for (lc = 0; lc < nl; lc += dl) {
                for (kc = 0; kc < nk; kc += dk) {
                        for (jc = 0; jc < nj; jc += dj) {
                                for (ic = 0; ic < ni; ic += di) {
        si2e_swap(tmp1, ox, oy, oz, o1, di, dj, nfk, nfl);
        (f_bra_spinor_si[k_l])(tmp2, d_l, tmp1, k_l, k_kp);
        (f_ket_spinor[l_l])(tmp1, d_k, tmp2, l_l, l_kp);
        pfijkl = fijkl + (ofl * lc + ofk * kc + ofj * jc + ic);

        zcopy_kijl(pfijkl, tmp1, ni, nj, nk, nl, di, dj, dk, dl);

        ox += nop;
        oy += nop;
        oz += nop;
        o1 += nop;
                                } } } }

        free(tmp1);
}
void c2s_si_2e2i(double complex *fijkl, const double complex *opij,
                 const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int k_sh = shls[2];
        const int l_sh = shls[3];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int k_l = bas(ANG_OF, k_sh);
        const int l_l = bas(ANG_OF, l_sh);
        const int i_kp = bas(KAPPA_OF, i_sh);
        const int j_kp = bas(KAPPA_OF, j_sh);
        const int k_kp = bas(KAPPA_OF, k_sh);
        const int l_kp = bas(KAPPA_OF, l_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int k_ctr = bas(NCTR_OF, k_sh);
        const int l_ctr = bas(NCTR_OF, l_sh);
        const int di = _len_spinor(i_l, i_kp);
        const int dj = _len_spinor(j_l, j_kp);
        const int dk = _len_spinor(k_l, k_kp);
        const int dl = _len_spinor(l_l, l_kp);
        const int ni = di * i_ctr;
        const int nj = dj * j_ctr;
        const int nk = dk * k_ctr;
        const int nl = dl * l_ctr;
        const int nfk = CINTlen_cart(k_l);
        const int nfl = CINTlen_cart(l_l);
        const int nf2k = nfk + nfk;
        const int nf2l = nfl + nfl;
        const int d_k = dk * di * dj;
        const int d_l = di * dj * nf2l;
        const int nop = nfk * di * dj * nfl;
        int ofj = ni;
        int ofk = ni * nj;
        int ofl = nk * ni * nj;
        int ic, jc, kc, lc;
        double complex *pfijkl;
        const double complex *ox = opij;
        const double complex *oy = ox + nop * i_ctr * j_ctr * k_ctr * l_ctr;
        const double complex *oz = oy + nop * i_ctr * j_ctr * k_ctr * l_ctr;
        const double complex *o1 = oz + nop * i_ctr * j_ctr * k_ctr * l_ctr;
        const int len1 = (nf2k*di*dj*nf2l + 16) & 0xfffffff0;
        const int len2 = dk*di*dj*nf2l;
        double complex *const tmp1 = malloc(sizeof(double complex)
                                            * (len1+len2));
        double complex *const tmp2 = tmp1 + len1;

        for (lc = 0; lc < nl; lc += dl) {
                for (kc = 0; kc < nk; kc += dk) {
                        for (jc = 0; jc < nj; jc += dj) {
                                for (ic = 0; ic < ni; ic += di) {
        si2e_swap(tmp1, ox, oy, oz, o1, di, dj, nfk, nfl);
        (f_bra_spinor_si[k_l])(tmp2, d_l, tmp1, k_l, k_kp);
        (f_iket_spinor[l_l])(tmp1, d_k, tmp2, l_l, l_kp);
        pfijkl = fijkl + (ofl * lc + ofk * kc + ofj * jc + ic);

        zcopy_kijl(pfijkl, tmp1, ni, nj, nk, nl, di, dj, dk, dl);

        ox += nop;
        oy += nop;
        oz += nop;
        o1 += nop;
                                } } } }

        free(tmp1);
}

/*
 * 1e integrals, reorder cartesian integrals.
 */
void c2s_cart_1e(double *opij, const double *gctr,
                 const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nf = nfi * nfj;
        const int ni = nfi * i_ctr;
        const int nj = nfj * j_ctr;
        int ic, jc;
        double *popij;

        for (jc = 0; jc < nj; jc += nfj)
                for (ic = 0; ic < ni; ic += nfi) {
                        popij = opij + ni * jc + ic;
                        dcopy_ij(popij, gctr, ni, nj, nfi, nfj);
                        gctr += nf;
                }
}

/*
 * 2e integrals, reorder cartesian integrals.
 */
void c2s_cart_2e1(double *fijkl, const double *gctr,
                  const int *shls, const int *bas)
{
        const int i_sh = shls[0];
        const int j_sh = shls[1];
        const int k_sh = shls[2];
        const int l_sh = shls[3];
        const int i_l = bas(ANG_OF, i_sh);
        const int j_l = bas(ANG_OF, j_sh);
        const int k_l = bas(ANG_OF, k_sh);
        const int l_l = bas(ANG_OF, l_sh);
        const int i_ctr = bas(NCTR_OF, i_sh);
        const int j_ctr = bas(NCTR_OF, j_sh);
        const int k_ctr = bas(NCTR_OF, k_sh);
        const int l_ctr = bas(NCTR_OF, l_sh);
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        const int nfk = CINTlen_cart(k_l);
        const int nfl = CINTlen_cart(l_l);
        const int ni = nfi * i_ctr;
        const int nj = nfj * j_ctr;
        const int nk = nfk * k_ctr;
        const int nl = nfl * l_ctr;
        const int nf = nfk * nfi * nfj * nfl;
        int ofj = ni;
        int ofk = ni * nj;
        int ofl = nk * ni * nj;
        int ic, jc, kc, lc;
        double *pfijkl;

        for (lc = 0; lc < nl; lc += nfl)
                for (kc = 0; kc < nk; kc += nfk)
                        for (jc = 0; jc < nj; jc += nfj)
                                for (ic = 0; ic < ni; ic += nfi) {
        pfijkl = fijkl + ofl * lc + ofk * kc + ofj * jc + ic;
        dcopy_kijl(pfijkl, gctr, ni, nj, nk, nl, nfi, nfj, nfk, nfl);
        gctr += nf;
                                }
}
void c2s_cart_2e2() {};



/*************************************************
 *
 * transform vectors
 *
 *************************************************/
double *CINTc2s_bra_sph(double *sph, int nket, double *cart, int l)
{
        return (f_bra_sph[l])(sph, nket, cart, l);
}
