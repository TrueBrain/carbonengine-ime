# Basic smoketests to make sure nothing crashes when called.

import unittest2
import inputmethod


class TestIME(unittest2.TestCase):
    def setUp(self):
        self.ime = inputmethod.Ime()

    def tearDown(self):
        del self.ime

    def testActivateKeyboardLayout(self):
        ret = self.ime.ActivateKeyboardLayout(0, 0)
        self.assertIsNone(ret)

    def testAssociateContext(self):
        ret = self.ime.AssociateContext(0)
        self.assertIsNone(ret)

    def testBackspace(self):
        ret = self.ime.Backspace(13)
        self.assertIsNone(ret)

    def testDisableTextFrameService(self):
        ret = self.ime.DisableTextFrameService()
        self.assertIsNone(ret)

    def testGetCandidateList(self):
        ret = self.ime.GetCandidateList()
        self.assertIsNone(ret)

    def testGetCompositionString(self):
        ret = self.ime.GetCompositionString(0)
        self.assertEqual(ret, {})

    def testGetConversionList(self):
        ret = self.ime.GetConversionList(u'a', 0)
        self.assertIsNone(ret)

    def testGetConversionStatus(self):
        ret = self.ime.GetConversionStatus()
        self.assertListEqual(ret, [0, 0])

    def testGetGuideLine(self):
        ret = self.ime.GetGuideLine(0)
        self.assertEqual(ret, 0)

    def testGetIMEFileName(self):
        ret = self.ime.GetIMEFileName()
        self.assertEqual(ret, '')

    def testGetImeId(self):
        ret = self.ime.GetImeId()
        self.assertEqual(ret, 0)

    def testGetKeyboardLayout(self):
        ret = self.ime.GetKeyboardLayout()
        self.assertIsInstance(ret, int)

    def testGetKeyboardLayoutList(self):
        ret = self.ime.GetKeyboardLayoutList()
        self.assertIsInstance(ret, list)
        self.assertGreater(len(ret), 0)

    def testGetOpenStatus(self):
        ret = self.ime.GetOpenStatus()
        self.assertEqual(ret, 0)

    def testGetReadingString(self):
        ret = self.ime.GetReadingString()
        self.assertDictEqual(ret, {'bShowReadingWindow': 0})

    def testGetRefCounts(self):
        ret = self.ime.GetRefCounts()
        self.assertTupleEqual(ret, (2L, 1))

    def testIsIME(self):
        ret = self.ime.IsIME()
        self.assertEqual(ret, 1)

    def testNotifyIME(self):
        ret = self.ime.NotifyIME(0, 0)
        self.assertEqual(ret, 0)

    def testOnLanguageChanged(self):
        ret = self.ime.OnLanguageChanged()
        self.assertIsNone(ret)

    def testSetCompositionString(self):
        ret = self.ime.SetCompositionString(0, u'a', u'b')
        self.assertEqual(ret, 0)

    def testSetConversionStatus(self):
        ret = self.ime.SetConversionStatus(0, 0)
        self.assertIsNone(ret)

    def testSetHWND(self):
        ret = self.ime.SetHWND(0)
        self.assertIsNone(ret)

    def testSetOpenStatus(self):
        ret = self.ime.SetOpenStatus(0)
        self.assertEqual(ret, 0)

    def testShowReadingWindow(self):
        ret = self.ime.ShowReadingWindow(0)
        self.assertIsNone(ret)

    def testSimulateHotKey(self):
        ret = self.ime.SimulateHotKey(0)
        self.assertEqual(ret, 0)