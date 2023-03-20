import unittest
import hakoc
import hako
import subprocess

class TestHako(unittest.TestCase):

 
    def test_asset_pdu_02(self):
        subprocess.run("../cleanup.bash")
        ret = hakoc.master_init()
        self.assertEqual(ret, True)
        hakoc.master_set_config_simtime(20 * 1000, 100 * 1000)


        offset_path = '/usr/local/lib/hakoniwa/py/hako_binary/offset'
        ctrl_01 = hako.Hako(offset_path, "ev3")
        ctrl_01.asset_name = 'TestAsset_01'
        ret = hakoc.asset_create_pdu_lchannel(ctrl_01.asset_name, 0, 256)
        self.assertEqual(ret, True)
        ret = hakoc.asset_register('TestAsset_01')
        self.assertEqual(ret, True)

        ctrl_02 = hako.Hako(offset_path, "ev3")
        ctrl_02.asset_name = 'TestAsset_02'
        ret = hakoc.asset_create_pdu_lchannel(ctrl_02.asset_name, 0, 256)
        self.assertEqual(ret, True)
        ret = hakoc.asset_register('TestAsset_02')
        self.assertEqual(ret, True)


        hakoc.start()
        hakoc.master_execute()
        ctrl_01.wait_event(hako.HakoEvent['START'])
        hakoc.master_execute()
        ctrl_02.wait_event(hako.HakoEvent['START'])
        hakoc.master_execute()
        ctrl_01.wait_state(hako.HakoState['RUNNING'])
        ctrl_02.wait_state(hako.HakoState['RUNNING'])

        binary_data_01 = bytearray(b"HELLO WORLD from asset-01")
        ret = hakoc.asset_write_pdu('TestAsset_01', 0, binary_data_01, len(binary_data_01))
        self.assertEqual(ret, True)

        binary_data_02 = bytearray(b"HELLO WORLD from asset-02")
        ret = hakoc.asset_write_pdu('TestAsset_02', 1, binary_data_02, len(binary_data_02))
        self.assertEqual(ret, True)

        read_buffer_01 = bytearray(len(binary_data_01))
        ret = hakoc.asset_read_pdu('TestAsset_01', 0, read_buffer_01, len(binary_data_01))
        self.assertEqual(ret, True)
        str1 = read_buffer_01.decode()
        print("result=" + str1)
        self.assertEqual(str1, "HELLO WORLD from asset-01")

        read_buffer_02 = bytearray(len(binary_data_02))
        ret = hakoc.asset_read_pdu('TestAsset_02', 1, read_buffer_02, len(binary_data_02))
        self.assertEqual(ret, True)
        str2 = read_buffer_02.decode()
        print("result=" + str2)
        self.assertEqual(str2, "HELLO WORLD from asset-02")

        subprocess.run("../cleanup.bash")
