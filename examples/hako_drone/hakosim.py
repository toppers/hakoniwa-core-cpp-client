import hakopy
import hako_pdu
import pdu_info

class MultirotorClient:
    def __init__(self, config_path):
        self.pdu_manager = None
        self.config_path = config_path

    def confirmConnection(self):
        self.pdu_manager = hako_pdu.HakoPduManager('/usr/local/lib/hakoniwa/hako_binary/offset', self.config_path)
        ret = hakopy.init_for_external()
        if ret == False:
            print(f"ERROR: init_for_external() returns {ret}.")
            return False
        return True

    def enableApiControl(self, v):
        pass
    def armDisarm(self, v):
        pass

    def get_packet(self, channel):
        command = self.pdu_manager.get_pdu('DroneFlightController', channel)
        cmd = command.get()
        cmd['header']['request'] = 1
        cmd['header']['result'] = 0
        cmd['header']['result_code'] = 0
        return command, cmd

    def reply_and_wait_res(self, command):
        ret = command.write()
        if ret == False:
            print('"ERROR: hako_asset_pdu_write')
            return False
        while True:
            pdu = command.read()
            if pdu == None:
                print('ERROR: hako_asset_pdu_read')
                return 0
            if pdu['header']['result'] == 1:
                pdu['header']['result'] = 0
                command.write()
                print('DONE')
                break
        return True

    def takeoff(self, height):
        print("INFO: takeoff")
        command, pdu_cmd = self.get_packet(pdu_info.HAKO_AVATOR_CHANNEL_ID_CMD_TAKEOFF)
        pdu_cmd['height'] = height
        pdu_cmd['speed'] = 5
        return self.reply_and_wait_res(command)

    def moveToPosition(self, x, y, z, speed):
        print("INFO: moveToPosition")
        command, pdu_cmd = self.get_packet(pdu_info.HAKO_AVATOR_CHANNEL_ID_CMD_MOVE)
        pdu_cmd['x'] = x
        pdu_cmd['y'] = y
        pdu_cmd['speed'] = speed
        return self.reply_and_wait_res(command)

    def land(self):
        print("INFO: moveToPosition")
        command, pdu_cmd = self.get_packet(pdu_info.HAKO_AVATOR_CHANNEL_ID_CMD_LAND)
        pdu_cmd['height'] = 0
        pdu_cmd['speed'] = 5
        return self.reply_and_wait_res(command)

