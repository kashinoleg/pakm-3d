import unittest
from TestUtils import GetTestTextile
from TexGen.TexGenv2 import ImportTexGenv2
from TexGen.Core import *

class ImportTest(unittest.TestCase):
    def tearDown(self):
        DeleteTextiles()

    def testImport(self):
        ImportTexGenv2('Data/test.pth')

class VerifyData(unittest.TestCase):
    def setUp(self):
        ImportTexGenv2('Data/test.pth')

    def tearDown(self):
        DeleteTextiles()

    def testNumYarns(self):
        self.assertEqual(GetTextile().GetNumYarns(), 8)

    def testVolume(self):
        textile = GetTextile()
        mesh = CMesh()
        textile.AddVolumeToMesh(mesh)
        self.assertAlmostEqual(mesh.CalculateVolume(), 3.9287158876148713)

def GetTestSuite():
    tests = []
    tests.append(unittest.TestLoader().loadTestsFromTestCase(ImportTest))
    tests.append(unittest.TestLoader().loadTestsFromTestCase(VerifyData))
    return unittest.TestSuite(tests)








