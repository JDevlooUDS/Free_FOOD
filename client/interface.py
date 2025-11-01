import subprocess
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import hashes, serialization

filePath = ""
countFiles1 = 0
countFiles2 = 0

def generate_private_key():
    private_key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
    return private_key

def get_public_key(private_key):
    return private_key.public_key()

def encode_public_key(public_key):
    pub_pem = public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )
    return pub_pem

def sign_file(file, private_key):
    signature = private_key.sign(
        file,
        padding.PKCS1v15(),
        hashes.SHA256()
    )
    return signature


def parcourir():
    global filePath
    global countFiles1 
    filePath = filedialog.askopenfilename()
    countFiles1 += 1
    listeFichiersC.insert(countFiles1, filePath)
    
    print("parcourir")
    
def compiler():
    #à faire
    #g++ file1.cpp file2.cpp -o myprogram
    selected_indices = listeFichiersC.curselection()
    selected_values = [listeFichiersC.get(i) for i in selected_indices]
    command1 = "cd "
    command2 = "g++ "

    for files in selected_values:
        command2 += files + " "
    command2 += "-o myprogram"
    print(command2)

    try:
        subprocess.run([command1], capture_output=True, text=True, check=True)
        result = subprocess.run([command2], capture_output=True, text=True, check=True)
        print("Command output:")
        print(result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {e}")
        print(f"Stderr: {e.stderr}")

    print("compiler")

def envoyer():
    #à faire
    print("envoyer")


from tkinter import *
from tkinter import filedialog


#widgets : buttons, text boxes, labels, images
#windows : container to hold these widget

window = Tk() #instantiate an instance of a window
screen_width = window.winfo_screenwidth()
screen_height = window.winfo_screenheight()

window.geometry(f"{screen_width}x{screen_height}") 
window.title("Shacks 2025") #à changer???

#label = Label(window,text="Hello world", font = ('Arial', 30, 'bold'), fg = 'green', bg = 'black', padx = 30, pady = 10)
#label.place(x = 0, y = 0)
#label.pack()
#
#entry = Entry()
#entry.config(font = ('Ink Free', 50))
#
#entry.pack()
#
#data_string = StringVar()
#data_string.set("Hello World! But, Wait!!! You Can Select Me :)")
#ent = Entry(window,textvariable=data_string,fg="black",bg="white",bd=0,state="readonly")
#ent.pack()

labelText = Label(window, text = "Déposez votre fichier :", font = ('Calibri', 20, 'bold'))
labelText.place(x = 15, y = 30)

listeFichiersC = Listbox(window, font = ('Calibri', 10), width = 90, selectmode= 'multiple')
listeFichiersC.place(x = 15, y = 80)

buttonParcourir = Button(window, text = "Parcourir", command=parcourir, font = ('Calibri', 10))
buttonParcourir.place(x = 25, y = 260)

buttonCompiler = Button(window, text = "Compiler", command=compiler, font = ('Calibri', 10))
buttonCompiler.place(x = 110, y = 260)

listeFichiersCompiles = Listbox(window, font = ('Calibri', 10), width = 90)
listeFichiersCompiles.place(x = 700, y = 80)

buttonEnvoyer = Button(window, text = "Envoyer", command=envoyer, font = ('Calibri', 10))
buttonEnvoyer.place(x = 710, y = 260)




window.mainloop() #place window on computer screen and listens for events
