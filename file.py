import cv2;
from math import sqrt

def hpf_coeffs(CL):
    N = len(CL)                    # Количество коэффициентов
    CH = [(-1)**k * CL[N - k - 1]  # Коэффициенты в обратном порядке с чередованием знака
        for k in range(N)]
    return CH

def icoeffs(CL, CH):
    assert(len(CL) == len(CH))         # Размеры списков коэффициентов должны быть равны
    iCL = []  # Коэффициенты первой строки
    iCH = []  # Коэффициенты второй строки
    for k in range(0, len(CL), 2):
        iCL.extend([CL[k-2], CH[k-2]])
        iCH.extend([CL[k-1], CH[k-1]])
    return (iCL, iCH)

def pconv(data, CL, CH, delta = 0):
    assert(len(CL) == len(CH))         # Размеры списков коэффициентов должны быть равны
    N = len(CL)
    M = len(data)
    out = []                           # Список с результатом, пока пустой
    for k in range(0, M, 2):  # Перебираем числа 0, 2, 4…
        sL = 0                         # Низкочастотный коэффициент
        sH = 0                         # Высокочастотный коэффициент
        for i in range(N):      # Находим сами взвешенные суммы
            sL += data[(k + i - delta) % M] * CL[i]
            sH += data[(k + i - delta) % M] * CH[i]
        out.append(sL)                 # Добавляем коэффициенты в список
        out.append(sH)
    return out

def dwt2(image, CL):
    CH = hpf_coeffs(CL)   # Вычисляем недостающие коэффициенты
    w = image.shape[1]
    h=image.shape[0]    # Размеры изображения
    imageT = image.copy() # Копируем исходное изображение для преобразования
    for i in range(h):   # Обрабатываем строки
        imageT[i, :] = pconv(imageT[i, :], CL, CH)
    for i in range(w):   # Обрабатываем столбцы
        imageT[:, i] = pconv(imageT[:, i], CL, CH)
    # Переупорядочиваем столбцы и строки
    data = imageT.copy()
    data[0:h/2, 0:w/2] = imageT[0:h:2, 0:w:2]
    data[h/2:h, 0:w/2] = imageT[1:h:2, 0:w:2]
    data[0:h/2, w/2:w] = imageT[0:h:2, 1:w:2]
    data[h/2:h, w/2:w] = imageT[1:h:2, 1:w:2]
    return data

CL = [(1 + sqrt(3)) / (4 * sqrt(2)),
    (3 + sqrt(3)) / (4 * sqrt(2)),
    (3 - sqrt(3)) / (4 * sqrt(2)),
    (1 - sqrt(3)) / (4 * sqrt(2))]
image = cv2.imread("bw.jpg")
data2 = dwt2(image, CL) 
cv2.imshow("Image", image)
cv2.waitKey(0)