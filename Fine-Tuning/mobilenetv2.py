from keras.applications.mobilenet_v2 import MobileNetV2

mnv2 = MobileNetV2(include_top=False,
                             weights='imagenet',
                             input_tensor=None,
                             input_shape=(299,299,3),
                             pooling=None,
                             classes=1000)

for layer in mnv2.layers[:-10]:
    layer.trainable = False


from keras import models
from keras import layers
from keras import optimizers

model = models.Sequential()
model.add(mnv2)

model.add(layers.Flatten())
model.add(layers.Dense(1024,activation='relu'))
model.add(layers.Dropout(0.5))
model.add(layers.Dense(3,activation='softmax'))
#model.summary()
train_dir=r'C:/Users/SISAQP-22/Documents/Tuneada/images_train'
valid_dir=r'C:/Users/SISAQP-22/Documents/Tuneada/images_valid'

from keras.preprocessing.image import ImageDataGenerator

train_datagen = ImageDataGenerator(
    rescale=1./255,
    rotation_range=20,
    width_shift_range=0.2,
    height_shift_range=0.2,
    horizontal_flip=True,
    fill_mode='nearest')
valid_datagen = ImageDataGenerator(rescale=1./255)
train_batchsize = 100
valid_batchsize = 10

train_gen = train_datagen.flow_from_directory(
    train_dir,
    target_size = (299,299),
    batch_size=train_batchsize,
    class_mode='categorical')

valid_gen = valid_datagen.flow_from_directory(
    valid_dir,
    target_size=(299,299),
    batch_size=valid_batchsize,
    class_mode='categorical',
    shuffle=False)

model.compile(loss='categorical_crossentropy',
              optimizer=optimizers.RMSprop(lr=1e-4),
              metrics=['acc'])


history = model.fit_generator(train_gen,
                              steps_per_epoch = train_gen.samples/train_gen.batch_size,
                              validation_data = valid_gen,
                              validation_steps = valid_gen.samples/valid_gen.batch_size,
                              epochs=20,
                              verbose=1,
                              workers=1,
                              use_multiprocessing=False)

model.save('last10.h5')