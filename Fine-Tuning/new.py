
import os
import sys
import glob
import argparse

import numpy as np
from tensorflow.python.keras import backend as K
from tensorflow.python.keras.models import load_model
from tensorflow.python.keras.applications.inception_resnet_v2 import preprocess_input
from tensorflow.python.keras.preprocessing import image

path = r'C:/Users/SISAQP-22/Documents/Tuneada/images_test'

def get_files(path):
    if os.path.isdir(path):
        files = glob.glob(os.path.join(path, '*'))
    elif path.find('*') > 0:
        files = glob.glob(path)
    else:
        files = [path]

    files = [f for f in files if f.endswith('JPG') or f.endswith('jpg')]

    if not len(files):
        sys.exit('No images found by the given path!')

    return files


if __name__ == '__main__':
    #args = parse_args()
    files = get_files(path)
    cls_list = ['Atypical Nevus', 'Common Nevus', 'Melanoma']

    # load the trained model
    net = load_model('last10.h5')

    # loop through all files and make predictions
    for f in files:
        img = image.load_img(f, target_size=(299,299))
        if img is None:
            continue
        x = image.img_to_array(img)
        x = preprocess_input(x)
        x = np.expand_dims(x, axis=0)
        pred = net.predict(x)[0]
        top_inds = pred.argsort()[::-1][:3]
        print(f)
        for i in top_inds:
            print('    {:.3f}  {}'.format(pred[i], cls_list[i]))
