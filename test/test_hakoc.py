import unittest
import hakoc
import hako

class TestHako(unittest.TestCase):

    def test_asset(self):
        ret = hakoc.master_init()
        self.assertEqual(ret, True)
        hakoc.master_set_config_simtime(20 * 1000, 100 * 1000)


        offset_path = '/usr/local/lib/hakoniwa/py/hako_binary/offset'
        ctrl = hako.Hako(offset_path, "ev3")
        ctrl.asset_name = 'TestAsset_01'
        ret = hakoc.asset_create_pdu_lchannel(ctrl.asset_name, 0, 256)
        self.assertEqual(ret, True)
        ret = hakoc.asset_register('TestAsset_01')
        self.assertEqual(ret, True)

        hakoc.start()
        hakoc.master_execute()
        ctrl.wait_event(hako.HakoEvent['START'])
        hakoc.master_execute()
        ctrl.wait_state(hako.HakoState['RUNNING'])

        binary_data = bytearray(b"HELLO WORLD")
        ret = hakoc.asset_write_pdu('TestAsset_01', 0, binary_data, len(binary_data))
        self.assertEqual(ret, True)

        read_buffer = bytearray(len(binary_data))
        ret = hakoc.asset_read_pdu('TestAsset_01', 0, read_buffer, len(binary_data))
        self.assertEqual(ret, True)
        str1 = read_buffer.decode()
        print("result=" + str1)
        self.assertEqual(str1, "HELLO WORLD")

