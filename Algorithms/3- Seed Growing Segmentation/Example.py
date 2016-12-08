
import cv
import cv2
import numpy as np
import tippy.segmentations as se
import tippy.basic_operations as bo
import tippy.display_operations as do


user_input = 1

img_name = "roi.bmp"
threshold = 20
connectivity = 4
img = cv.LoadImage(img_name, cv.CV_LOAD_IMAGE_GRAYSCALE)
seed = (10, 10);

out_img = se.simple_region_growing(img, seed, threshold, connectivity)

#do.display_single_image(out_img, "Region Growing result")
cv.SaveImage('hj.bmp',out_img );
    
