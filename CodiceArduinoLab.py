import serial
import dearpygui.dearpygui as dpg

# Per utilizzare il codice è necessario scaricare le librerie DeerPyGui e Serial anche se quest'ultima dovreste averla tutti se avete fatto il progetto con python,

# potete comodamente scaricare le librerie dalla schermata Python packeges di PyCharm

arduino = serial.Serial(port='COM5', baudrate=38400, timeout=.1) # MODIFICARE IL VALORE DI port IN BASE ALLA QUALE SI È SCELTO DI USARE


dpg.create_context()

# limite massimo di dati che gestirà python, NON é il limite di arduino
max_data_width = 100

# Inizializzazione dei dati che saranno poi ricevuti da arduino
datax = [0] * max_data_width
datay1 = [0] * max_data_width
datay2 = [0] * max_data_width

graph_data = []

is_running = True

# Aggiornamento dei dati del grafico
def update_y(points):
    # "points" è un array che per ogni entrata ha una stringa del tipo "x,y1,y2", ovvero uno dei punti da plottare

    input_length = len(points)

    if (input_length == 0):
        return

    data_width = min(max_data_width, input_length) # Si sceglie quanto devono essere lunghi gli array dei dati


    datax = [0] * data_width
    datay1 = [0] * data_width
    datay2 = [0] * data_width

    for i in range(data_width): # Si salvano tutti i punti nei rispettivi array che verranno poi gestiti da DPG
        t = points[i].strip().split(",")

        try:
            datax[i] = int(t[0])
            datay1[i] = int(t[1])
            datay2[i] = int(t[2])
        except:
            print("Conversione int fallita (probabilmente)")
            return -1

    # Aggiornamento delle due funzioni nella schermata grafica
    dpg.set_value("data_axis1", [datax, datay1, [], [], []])
    dpg.set_value("data_axis2", [datax, datay2, [], [], []])


    # Aggiornamento dei limiti degli assi
    dpg.set_axis_limits("x_axis", min(datax), max(datax))

    y_max = max(max(datay1), max(datay2))
    y_min = min(min(datay1), min(datay2))

    dpg.set_axis_limits("y_axis", y_min - y_min * 0.1, y_max + y_max * 0.1)


# Funzione che gestisce la pausa dell'acquisizione dei dati
def pause_data_acquisition():
    global is_running
    is_running = not is_running
    if is_running:
        dpg.set_value("tag_running", "Running")
        dpg.configure_item("tag_running", color=(0, 255, 0, 255))
    else:
        dpg.set_value("tag_running", "Paused")
        dpg.configure_item("tag_running", color=(255, 0, 0, 255))


# Inizializzazione della schermata utlizzando DPG
with dpg.window():
    dpg.set_primary_window(dpg.last_item(), True)

    with dpg.menu_bar():
        dpg.add_menu_item(label="Pause", callback=pause_data_acquisition)
        dpg.add_text("Running", color=(0, 255, 0, 255), tag="tag_running")
    with dpg.plot(anti_aliased=True, width=-1, height=-1):
        dpg.add_plot_axis(0, tag="x_axis")
        with dpg.plot_axis(1, tag="y_axis"):
            dpg.add_line_series(datax, datay1, tag="data_axis1")
            dpg.add_line_series(datax, datay2, tag="data_axis2")

        dpg.set_axis_limits("x_axis", 0, 100)
        dpg.set_axis_limits("y_axis", 0, 100)

dpg.create_viewport(title="Oscilloscope", width=500, height=340)
dpg.setup_dearpygui()
dpg.show_viewport()

# Loop dove vengono acquisiti i dati da arduino
while dpg.is_dearpygui_running():

    data = arduino.readline().strip() # Ricezioni dei dati tramite la libreria Serial

    if is_running:
        temp = lambda d: d[:-1].decode("ascii").split(';') if len(d) != 0 else [] # Trasformazione dei dati da una stringa ad un array,
        graph_data = temp(data)                                                   # se "data" contiene dati, altrimenti ritorna un array vuoto

    dpg.render_dearpygui_frame()

    # Aggiornamento del grafico con i nuovi punti
    update_y(graph_data)

dpg.destroy_context()