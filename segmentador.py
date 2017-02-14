import cv2
import numpy as np
import math as m
class segmentador ():

    def __init__(self, video):

        self.umbral=254
        self.frame_ant=None
        self.frame=None
        self.fondo=None
        self.acum=None
        self.frente=None
        self.alpha=0.05
        self.mask=None
        self._in=video
        self.metodo=None
        self.metodo_procesa=None
        self.vin=cv2.VideoCapture(self._in)

    def asigna_metodo(self,metodo):

        self.metodo=metodo
        frame = self.captura()
        self.metodo(frame,init=True)

    def captura(self):

        ret, frame = self.vin.read()
        return frame

    def segmenta(self,frame=None,init=None):
        self.metodo(frame,init)

    def procesa(self):

        self.metodo_procesa()

    def procesa_media_movil(self):

        while (True):
            self.frame_ant=np.copy(self.frame)
            self.frame = self.captura()
            if self.frame is None:
                break
            original=np.copy(self.frame)
            self.segmenta(self.frame)
            if len(self.frame.shape)==3:
                self.frame[:,:,0]=np.multiply(self.frame[:,:,0],self.mask)
                self.frame[:,:,1] = np.multiply(self.frame[:, :, 1], self.mask)
                self.frame[:,:,2] = np.multiply(self.frame[:, :, 2], self.mask)
            else:
                self.frame=np.multiply(self.frame,self.mask)
            self.acum=(self.alpha*self.frame)+(1-self.alpha)*self.acum
            cv2.imshow('resultado',self.frame)
            cv2.imshow('video', original)
            tecla=cv2.waitKey(15)

            #if tecla !=-1:
            if tecla !=255:
                print tecla
                break

    def procesa_fondo_estatico(self):

        frame=self.captura()
        self.segmenta(frame,init=True)

        while (True):

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

            cv2.imshow('resultado',frame)
            cv2.imshow('video', original)
            tecla=cv2.waitKey(15)

            if tecla == 1048689:
                break
            if tecla == 1048686:
                self.fondo = self.captura()
    def procesa_frame_anterior(self):

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

            cv2.imshow('resultado',frame)
            cv2.imshow('video', original)
            tecla=cv2.waitKey(15)

            if tecla != 255:
                break

    def fondo_frame_anterior(self,frame=None,init=None):

        if init is True:
            self.acum=frame
            self.metodo_procesa=self.procesa_frame_anterior

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
            self.metodo_procesa=self.procesa_media_movil

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
            self.acum = frame
            self.metodo_procesa=self.procesa_fondo_estatico
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