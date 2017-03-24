import numpy as np
import scipy.signal as si
#from PIL import Image


def gauss_kern():
    h1 = 15
    h2 = 15
    x, y = np.mgrid[0:h2, 0:h1]
    x = x - h2 / 2
    y = y - h1 / 2
    sigma = 1.5
    g = np.exp(-(x ** 2 + y ** 2) / (2 * sigma ** 2));
    return g / g.sum()


def deriv(im1, im2):
    g = gauss_kern()
    Img_smooth = si.convolve(im1, g, mode='same')
    fx, fy = np.gradient(Img_smooth)
    ft = si.convolve2d(im1, 0.25 * np.ones((2, 2))) + \
         si.convolve2d(im2, -0.25 * np.ones((2, 2)))

    fx = fx[0:fx.shape[0] - 1, 0:fx.shape[1] - 1]
    fy = fy[0:fy.shape[0] - 1, 0:fy.shape[1] - 1];
    ft = ft[0:ft.shape[0] - 1, 0:ft.shape[1] - 1];
    return fx, fy, ft


import matplotlib.pyplot as plt
import numpy as np
import scipy.signal as si
from PIL import Image
import deriv
import numpy.linalg as lin


def lk(im1, im2, i, j, window_size):
    fx, fy, ft = deriv.deriv(im1, im2)
    halfWindow = np.floor(window_size / 2)
    curFx = fx[i - halfWindow - 1:i + halfWindow,
            j - halfWindow - 1:j + halfWindow]
    curFy = fy[i - halfWindow - 1:i + halfWindow,
            j - halfWindow - 1:j + halfWindow]
    curFt = ft[i - halfWindow - 1:i + halfWindow,
            j - halfWindow - 1:j + halfWindow]
    curFx = curFx.T
    curFy = curFy.T
    curFt = curFt.T

    curFx = curFx.flatten(order='F')
    curFy = curFy.flatten(order='F')
    curFt = -curFt.flatten(order='F')

    A = np.vstack((curFx, curFy)).T
    U = np.dot(np.dot(lin.pinv(np.dot(A.T, A)), A.T), curFt)
    return U[0], U[1]