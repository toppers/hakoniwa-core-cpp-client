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
import time

# データ用キュー
data_queue = Queue(maxsize=1)
pdu_manager = None

class LidarVisualizer(QMainWindow):
    def __init__(self):
        super().__init__()

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

    def update_plot(self):
        global data_queue
        do_read_lidar_scan()
        if not data_queue.empty():
            # キューからデータを取得
            data = data_queue.get()
            ranges_data = data['ranges']
            angle_min = data['angle_min']
            angle_max = data['angle_max']

            if len(ranges_data) > 0:
                angles = np.linspace(angle_min, angle_max, len(ranges_data))
                angles_rad = np.deg2rad(angles)
                x = np.array(ranges_data) * np.cos(angles_rad)
                y = np.array(ranges_data) * np.sin(angles_rad)
                self.scatter.setData(x, y, size=2)  # 点の大きさを調整

def do_read_lidar_scan():
    if hakopy.simulation_time() <= 1000000:
        return
    global pdu_manager, data_queue
    pdu = pdu_manager.get_pdu("LiDAR2D", 0)
    scan = pdu.read()
    if scan:
        # データの検証を追加
        ranges = scan.get('ranges')
        #print(f'ranges: {ranges}')
        angle_min_raw = scan.get('angle_min')
        angle_max_raw = scan.get('angle_max')

        if ranges is None or angle_min_raw is None or angle_max_raw is None:
            print("LiDAR data is incomplete or missing.")
            return 0

        if data_queue.full():
            data_queue.get()  # 最古のデータを削除
        data_queue.put({
            'ranges': ranges,
            'angle_min': np.rad2deg(angle_min_raw),
            'angle_max': np.rad2deg(angle_max_raw)
        })
    else:
        print("LiDAR data not available.")
    return 0


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <config_path>")
        return 1
    global pdu_manager
    asset_name = 'LidarPython'
    config_path = sys.argv[1]
    delta_time_usec = 20000

    pdu_manager = hako_pdu.HakoPduManager('/usr/local/lib/hakoniwa/hako_binary/offset', config_path)
    hakopy.conductor_start(delta_time_usec, delta_time_usec)

    ret = hakopy.init_for_external()
    if not ret:
        print(f"ERROR: init_for_external() returns {ret}.")
        return 1

    app = QApplication(sys.argv)
    visualizer = LidarVisualizer()
    visualizer.show()
    sys.exit(app.exec_())
    hakopy.conductor_stop()
    return 0

if __name__ == "__main__":
    main()
