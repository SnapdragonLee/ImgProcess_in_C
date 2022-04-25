import matplotlib.pyplot as plt
import numpy as np

from testing.imgConfig import imgPath

if __name__ == "__main__":
    img_start = plt.imread(imgPath)
    f = np.fft.fft2(img_start)
    f = np.fft.fftshift(f)

    # magnitude_spectrum = 20*np.log(np.abs(f))
    magnitude_spectrum = np.abs(f)

    plt.subplot(), plt.imshow(magnitude_spectrum, cmap='gray')

    f_ishift = np.fft.ifftshift(f)
    img_back = np.fft.ifft2(f)
    img_back = np.abs(img_back)

    # plt.imshow(img_back, cmap = 'gray')
