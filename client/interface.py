import subprocess

filePath = ""
countFiles1 = 0
bgColour = "#1b1f33"
bgColourLight = "#3d456e"
colour2 = "#7ccbe6"
colour2light = "#bdeeff"


def parcourir():
    global filePath
    global countFiles1 
    filePath = filedialog.askdirectory()

    if filePath != "":
        countFiles1 += 1
        listeFichiersCompiles.insert(countFiles1, filePath + "/.pio/build/megaatmega2560/firmware.hex")
    
    print("parcourir")
    

def envoyer():
    #à faire
    print("envoyer")

def listboxSelect(event):
    buttonEnvoyer.config(state = "normal")
    buttonEnvoyer.config(bg = colour2)


from tkinter import *
from tkinter import filedialog


#widgets : buttons, text boxes, labels, images
#windows : container to hold these widget

window = Tk() #instantiate an instance of a window
screen_width = window.winfo_screenwidth()
screen_height = window.winfo_screenheight()

window.geometry(f"{screen_width}x{screen_height}") 
window.title("Shacks 2025") #à changer???
window.config(bg = bgColour)

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
labelText = Label(window, text = "Veuillez choisir le dossier du projet", font = ('Calibri', 11), bg = bgColour, fg = colour2)
labelText.place(x = 15, y = 13)

labelTextGros = Label(window, text = "Projet compilé :", font = ('Calibri', 20, 'bold'), bg = bgColour, fg = colour2)
labelTextGros.place(x = 15, y = 35)

labelBorder = Label(window, bg = colour2, padx = 80, pady = 15)
labelBorder.place(x = 22, y =257)

buttonParcourir = Button(window, 
    text = "Choisir le dossier", 
    command=parcourir, 
    font = ('Calibri', 15, 'bold'),
    bg = bgColour,
    fg = colour2,
    #activeforeground = colour2light,
    #borderwidth = 0,
    #highlightbackground = colour2
    )
buttonParcourir.place(x = 25, y = 260)

listeFichiersCompiles = Listbox(window, font = ('Calibri', 10), width = 130)
listeFichiersCompiles.place(x = 15, y = 80)
listeFichiersCompiles.bind('<<ListboxSelect>>', listboxSelect)


buttonEnvoyer = Button(window, 
    text = "Envoyer au arduino", 
    command=envoyer, 
    font = ('Calibri', 15, 'bold'), 
    state = "disabled",
    bg = colour2light,
    fg = bgColour,
    activebackground = colour2light,
    disabledforeground = bgColourLight,
    highlightthickness = 3,
    highlightbackground = bgColour
    )
buttonEnvoyer.place(x = 195, y = 260)






window.mainloop() #place window on computer screen and listens for events
