import cv2

class segmentador ():

    def __init__(self, background_method,video):

        self.method=background_method
        self.background=None
        self.foreground=None
        self.alpha=None
        self.mask=None
        self._in=video
        self.vin=cv2.VideoCapture(self._in)

    def captura(self):

        ret, frame = self.vin.read()
        return frame

    def segmenta(self,img):

        self.method(img)

    def procesa(self):
        while (True):
