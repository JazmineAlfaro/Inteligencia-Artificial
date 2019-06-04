import keras

from keras.applications.mobilenetv2 import MobileNetV2
from keras.preprocessing import image
from keras.applications.mobilenetv2 import preprocess_input
from keras.applications.mobilenetv2 import decode_predictions

import numpy as np

model =  MobileNetV2(weights='imagenet')

image_path = 'elephant.jpg'

#image_path = 'oso.jpg'


img = image.load_img(image_path,target_size=(224,224))

x=image.img_to_array(img)

x=np.expand_dims(x,axis=0)

x=preprocess_input(x)

preds = model.predict(x)


for index, res in enumerate(decode_predictions(preds,top=10)[0]):
    print('{}. {}: {:.3f}%'.format(index + 1, res[1], 100 * res[2]))


#print(':',decode_predictions(preds,top=10)[0])
