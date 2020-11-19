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
	image = Image.new("1", (img_size, img_size), 1)
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
	s = c.lower()
	
	if c.isdigit():
		s += ""+extension
	elif c.islower():
		s += "_l"+extension
	else:
		s += "_u"+extension

	return s




def create_datasets(path, fontpath, img_size, offsets = [], rotations = []):

	img_font = ImageFont.truetype(fontpath, size = img_size // 2 + 2)

	for c in chars:

		im_name = get_name(c, ".bmp")

		im = create_letter_image(c, img_font, img_size)
		im.save(os.path.join(path, im_name))

	if offsets != []:

		# offsetted
		for c in chars:
			for (i,j) in offsets:

				im_name = get_name(c, "_X{0}Y{0}.bmp".format(i,j))

				im = create_letter_image(c, img_font, img_size, Xoff = i, Yoff = j)
				im.save(os.path.join(path, im_name))

	if rotations != []:
		# rotated
		for c in chars:
			for theta in rotations:

				im_name = get_name(c, "_R{}.bmp".format(theta))

				im = create_letter_image(c, img_font, img_size, Xoff = 0, Yoff = 0, angle = theta)
				im.save(os.path.join(path, im_name))



folder = input("path to save the dataset: ")
ft_path = input("path to the font file: ")

default_size = 30
default_offsets = [(1,1),(-1,1),(1,-1),(-1,-1)]
default_angles = [-10,10]

create_datasets(folder, ft_path, default_size, default_offsets, default_angles)

print("Created a dataset with :\n- 1 normal set")
print("- {} shifted sets".format(len(default_offsets)))
print("- {} rotated sets".format(len(default_angles)))
