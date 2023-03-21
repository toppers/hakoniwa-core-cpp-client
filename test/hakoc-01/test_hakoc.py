import unittest
import hakoc
import hako

class TestHako(unittest.TestCase):

 
    def test_asset_pdu_01(self):
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

        hakoc.start()
        hakoc.master_execute()
        ctrl_01.wait_event(hako.HakoEvent['START'])
        hakoc.master_execute()
        ctrl_01.wait_state(hako.HakoState['RUNNING'])

        binary_data_01 = bytearray(b"HELLO WORLD from asset-01")
        ret = hakoc.asset_write_pdu('TestAsset_01', 'robo_01', 0, binary_data_01, len(binary_data_01))
        self.assertEqual(ret, True)

        read_buffer_01 = bytearray(len(binary_data_01))
        ret = hakoc.asset_read_pdu('TestAsset_01', 'robo_01', 0, read_buffer_01, len(binary_data_01))
        self.assertEqual(ret, True)
        str1 = read_buffer_01.decode()
        print("result=" + str1)
        self.assertEqual(str1, "HELLO WORLD from asset-01")


