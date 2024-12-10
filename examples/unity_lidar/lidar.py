import sys
import threading
import numpy as np
from queue import Queue
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget
from PyQt5.QtCore import QTimer
from pyqtgraph import PlotWidget, ScatterPlotItem
import pyqtgraph as pg
import hakopy
import hako_pdu
import os

class LidarVisualizer(QMainWindow):
    def __init__(self, mgr):
        super().__init__()
        self.start_time_usec = 1000000
        self.data_queue = Queue(maxsize=1)
        self.pdu_manager = mgr
        self.scan_data = None

        self.setWindowTitle("Real-time LiDAR Data")

        # Create a widget for the main window
        self.main_widget = QWidget()
        self.setCentralWidget(self.main_widget)

        # Create a layout for the widget
        layout = QVBoxLayout()
        self.main_widget.setLayout(layout)

        # Create a plot widget
        self.plot_widget = PlotWidget()
        self.scatter = pg.ScatterPlotItem()
        self.plot_widget.addItem(self.scatter)
        self.plot_widget.setXRange(-0.1, 6)
        self.plot_widget.setYRange(-2, 2)
        self.plot_widget.setLabel('left', "Y (meters)")
        self.plot_widget.setLabel('bottom', "X (meters)")
        self.plot_widget.setTitle("Real-time LiDAR Data")
        layout.addWidget(self.plot_widget)

        # Timer to update the plot
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_plot)
        self.timer.start(100)  # Update every 100ms

    def scan(self):
        if hakopy.simulation_time() > self.start_time_usec:
            pdu = self.pdu_manager.get_pdu("LiDAR2D", 0)
            self.scan_data = pdu.read()

    def update_plot(self):
        self.scan()
        if self.scan_data is not None:
            ranges_data = self.scan_data['ranges']
            angle_min = np.rad2deg(self.scan_data['angle_min'])
            angle_max = np.rad2deg(self.scan_data['angle_max'])

            if len(ranges_data) > 0:
                angles = np.linspace(angle_min, angle_max, len(ranges_data))
                angles_rad = np.deg2rad(angles)
                x = np.array(ranges_data) * np.cos(angles_rad)
                y = np.array(ranges_data) * np.sin(angles_rad)
                self.scatter.setData(x, y, size=2)

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <config_path>")
        return 1
    asset_name = 'LidarPython'
    config_path = sys.argv[1]
    delta_time_usec = 20000

    hako_binary_path = os.getenv('HAKO_BINARY_PATH', '/usr/local/lib/hakoniwa/hako_binary/offset')
    pdu_manager = hako_pdu.HakoPduManager(hako_binary_path, config_path)
    hakopy.conductor_start(delta_time_usec, delta_time_usec)

    ret = hakopy.init_for_external()
    if not ret:
        print(f"ERROR: init_for_external() returns {ret}.")
        return 1

    app = QApplication(sys.argv)
    visualizer = LidarVisualizer(pdu_manager)
    visualizer.show()
    app.aboutToQuit.connect(hakopy.conductor_stop)
    sys.exit(app.exec_())
    return 0

if __name__ == "__main__":
    main()
