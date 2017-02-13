import cv2
import numpy as np
import math as m
class segmentador ():

    def __init__(self, video):

        self.umbral=250
        self.frame_ant=None
        self.fondo=None
        self.acum=None
        self.frente=None
        self.alpha=0.5
        self.mask=None
        self._in=video
        self.vin=cv2.VideoCapture(self._in)

    def asigna_metodo(self,metodo):

        self.metodo=metodo


    def captura(self):

        ret, frame = self.vin.read()
        return frame

    def segmenta(self,frame=None,init=None):

        self.metodo(frame,init)

    def procesa(self):

        frame=self.captura()
        self.segmenta(frame,init=True)

        while (True):
            self.frame_ant=np.copy(frame)
            frame = self.captura()
            if frame is None:
                break
            self.segmenta(frame)
            original=np.copy(frame)
            if len(frame.shape)==3:
                frame[:,:,0]=np.multiply(frame[:,:,0],self.mask)
                frame[:,:,1] = np.multiply(frame[:, :, 1], self.mask)
                frame[:,:,2] = np.multiply(frame[:, :, 2], self.mask)
            else:
                frame=np.multiply(frame,self.mask)
            self.acum=(self.alpha*frame)+(1-self.alpha)*self.acum
            cv2.imshow('resultado',frame)
            cv2.imshow('video', original)
            tecla=cv2.waitKey(15)
            if tecla != -1:
                break
    def fondo_frame_anterior(self,frame=None,init=None):


        if init is None:
            self.mask = np.zeros(frame.shape, dtype=bool)

            if len(frame.shape)==3:
                index=2
            else:
                index=0

            while index>0:
                tmp=self.mask[:,:,index]
                tmp[np.absolute(np.subtract(frame[:,:,index],self.frame_ant[:,:,index]))>self.umbral]=True
                self.mask[:,:,index]=tmp
                index=index-1

            if len(self.mask.shape)==3:
                tmp=np.logical_or(self.mask[:,:,0],self.mask[:,:,1])
                self.mask=np.logical_or(tmp,self.mask[:,:,2])

    def fondo_media_movil(self,frame=None,init=None):

        if init is True:
            self.acum = frame

        if init is None:
            self.mask = np.zeros(frame.shape, dtype=bool)

            if len(frame.shape)==3:
                index=2
            else:
                index=0

            while index>0:
                tmp=self.mask[:,:,index]
                tmp[np.absolute(np.subtract(frame[:,:,index],self.acum[:,:,index]))>self.umbral]=True
                self.mask[:,:,index]=tmp
                index=index-1

            if len(self.mask.shape)==3:
                tmp=np.logical_or(self.mask[:,:,0],self.mask[:,:,1])
                self.mask=np.logical_or(tmp,self.mask[:,:,2])

    def fondo_estatico(self, frame=None, init=None):

        if init is True:
            self.fondo = frame
            cv2.imshow('Imagen de fondo', frame)
            cv2.waitKey()
            cv2.destroyAllWindows()
        if init is None:
            self.mask = np.zeros(frame.shape, dtype=bool)

            if len(frame.shape) == 3:
                index = 2
            else:
                index = 0

            while index > 0:
                tmp = self.mask[:, :, index]
                aux = np.subtract(frame[:, :, index], self.fondo[:, :, index])
                tmp[np.absolute(np.subtract(frame[:, :, index], self.fondo[:, :, index])) > self.umbral] = True
                self.mask[:, :, index] = tmp
                index = index - 1

            if len(self.mask.shape) == 3:
                tmp = np.logical_or(self.mask[:, :, 0], self.mask[:, :, 1])
                self.mask = np.logical_or(tmp, self.mask[:, :, 2])