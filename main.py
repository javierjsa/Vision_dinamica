from segmentador import segmentador
import numpy as np
import argparse
import math as m
import cv2

#fondo estatico=umbral 250
#fondo frame anterior=225
def main():

    ap = argparse.ArgumentParser()
    ap.add_argument("-v", "--video", required=True, help="video de entrada")
    ap.add_argument("-o", "--out", required=False, help="video de salida")
    args = vars(ap.parse_args())

    _in = args['video']
    _out = args['out']


    s=segmentador(_in)
    #s.asigna_metodo(s.fondo_frame_anterior)
    #s.asigna_metodo(s.fondo_estatico)
    s.asigna_metodo(s.fondo_media_movil)
    s.procesa()


def method (img):

    cv2.imshow('imagen', img)
    cv2.waitKey(0)


if __name__ == "__main__":
    main()
