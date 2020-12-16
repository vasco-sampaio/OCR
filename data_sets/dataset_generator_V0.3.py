"""

Credits:
Dorian 'Renji' PERON - dorian.peron@epita.fr

2020

"""

from PIL import Image, ImageDraw, ImageFont, ImageOps
import os
import sys


chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"


def create_letter_image(letter, imgfont, img_size, Xoff = 0, Yoff = 0, angle = 0):

    # create a white binary image of img_size 41*41 
    image = Image.new("RGB", (img_size, img_size), 'rgb(255,255,255)')
    img_draw = ImageDraw.Draw(image)

    (imX, imY) = image.size
    (tX, tY) = img_draw.textsize(letter, imgfont)

    img_draw.text(((imX - tX)/2 + Xoff,(imY - tY)/2 + Yoff), letter, font = imgfont, fill = 'rgb(0,0,0)')

    if angle == 0:
        return image
    var = image.rotate(angle, fillcolor = 'rgb(255,255,255)')

    return var



# Just define the name of a file
#
# -> if letter "_l" for lowercase, "_u" for uppercase
# -> then add extension
#
def get_name(c, extension):
    s = c
    return s + extension



def create_datasets(path, fonts, img_size):

    for (fontpath, font_size) in fonts:

        #font_size = img_size // 2 + 2
        img_font = ImageFont.truetype(fontpath, size = font_size)
        ft_name = fontpath.split('.')[0]

        for c in chars:
            im_name = get_name(c, '_'+ft_name+".bmp")
            im = create_letter_image(c, img_font, img_size)
            f = open(os.path.join(path, im_name), "wb")

            im.save(f, "bmp")
            f.close()

# Where to save the images
folder = "arial_25_train"

# path to the font file
fonts = [("arial.ttf", 30)]

# Image size
default_size = 30

if not os.path.exists(folder):
    os.makedirs(folder)

create_datasets(folder, fonts, default_size)

