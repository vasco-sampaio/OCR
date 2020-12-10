from PIL import Image, ImageDraw, ImageFont, ImageOps
import os
import sys

"""

Intellectual property of :
Dorian 'Renji' PERON - dorian.peron@epita.fr

2020

"""

chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"


def create_letter_image(letter, imgfont, img_size, Xoff = 0, Yoff = 0, angle = 0):

        # create a white binary image of img_size 41*41 
        image = Image.new("RGB", (img_size, img_size), 1)
        img_draw = ImageDraw.Draw(image)

        (imX, imY) = image.size
        (tX, tY) = img_draw.textsize(letter, imgfont)

        img_draw.text(((imX - tX)/2 + Xoff,(imY - tY)/2 + Yoff), letter, font = imgfont, fill = 'rgb(0,0,0)')

        if angle == 0:
                return image
        else:
                var = image.rotate(angle, fillcolor = 1)

                return var



# Just define the name of a file
#
# -> if letter "_l" for lowercase, "_u" for uppercase
# -> then add extension
#
def get_name(c, extension):
        s = c
        return s + extension
       


def create_datasets(path, fontpath, img_size, font_size = None, offsets = [], rotations = []):

        if not font_size:
                font_size = img_size // 2 + 2
        img_font = ImageFont.truetype(fontpath, size = font_size)

        for c in chars:

                im_name = get_name(c, ".bmp")
                im = create_letter_image(c, img_font, img_size)
                f = open(os.path.join(path, im_name), "wb")

                im.save(f, "bmp")
                f.close()

        if offsets != []:

                # offsetted
                for c in chars:
                        for (i,j) in offsets:

                                im_name = get_name(c, "_X{0}Y{1}.bmp".format(i,j))

                                im = create_letter_image(c, img_font, img_size, Xoff = i, Yoff = j)

                                f = open(os.path.join(path, im_name), "wb")
                                im.save(f, "bmp")
                                f.close()

        if rotations != []:
                # rotated
                for c in chars:
                        for theta in rotations:

                                im_name = get_name(c, "_R{}.bmp".format(theta))

                                im = create_letter_image(c, img_font, img_size, Xoff = 0, Yoff = 0, angle = theta)
                                im.save(os.path.join(path, im_name))


# Where to save the images
folder = "arial_25_train"

# path to the font file 
ft_path = "arial.ttf"
# font size
f_size = 25

# Image size
default_size = 30
# Additional offsetted values
default_offsets = [(1,1),(-1,1),(1,-1),(-1,-1)]
# Additional offsetted 
default_angles = [-10,10]

if not os.path.exists(folder):
        os.makedirs(folder)

create_datasets(folder, ft_path, default_size, font_size = f_size, offsets=default_offsets, rotations=default_angles)

print("Created a dataset with :\n- 1 normal set")
print("- {} shifted sets".format(len(default_offsets)))
print("- {} rotated sets".format(len(default_angles)))
