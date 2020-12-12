import cv2
import numpy as np
import face_recognition 
import os
from PIL import Image

number_of_clients = 0

base_dir = os.path.dirname(os.path.abspath(__file__))
image_dir = os.path.join(base_dir, "images")

imge = face_recognition.load_image_file(r"C:\Users\wemer\Desktop\projeto_embarcados\python\images\9\9.jpg")
imge = cv2.cvtColor(imge, cv2.COLOR_BGR2RGB)
faceloce = face_recognition.face_locations(imge)[0]
encodeimge = face_recognition.face_encodings(imge)[0]
cv2.rectangle(imge, (faceloce[3], faceloce[0]), (faceloce[1], faceloce[2]), (255, 0, 255), 2)



for root, dirs, files in os.walk(image_dir):
    for file in files:  
        if file.endswith("png") or file.endswith("jpg"):
            path = os.path.join(root, file)
            #print(path)
            label = os.path.basename(root).replace(" ", "-").lower()
            #print(label,path)
            img = face_recognition.load_image_file(path)
            img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB) 
            faceloc = face_recognition.face_locations(img)[0]  
            encodeimg = face_recognition.face_encodings(img)[0] 
            comparacao1 = face_recognition.compare_faces([encodeimg],encodeimge) 
            comp_dis = face_recognition.face_distance([encodeimg],encodeimge)
            #print(comp_dis)
            print(comparacao1)
            match_index = np.argmin(comp_dis)
            if comparacao1[match_index]:
                os.remove(path)
                print("deletada")
                number_of_clients = number_of_clients+1
                break
                
print(number_of_clients)