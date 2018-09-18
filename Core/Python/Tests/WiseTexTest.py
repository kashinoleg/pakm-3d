import unittest
from TestUtils import GetTestTextile
from TexGen.WiseTex import ImportWiseTex
from TexGen.Core import *

class ImportTest(unittest.TestCase):
    def tearDown(self):
        DeleteTextiles()

    def testImport(self):
        ImportWiseTex('Data/test.cfl')

class VerifyData(unittest.TestCase):
    def setUp(self):
        ImportWiseTex('Data/test.cfl')

    def tearDown(self):
        DeleteTextiles()

    def testNumYarns(self):
        self.assertEqual(GetTextile().GetNumYarns(), 4)

    def testVolume(self):
        textile = GetTextile()
        mesh = CMesh()
        textile.AddVolumeToMesh(mesh)
        self.assertAlmostEqual(mesh.CalculateVolume(), 7.6144607823548363)

def GetTestSuite():
    tests = []
    tests.append(unittest.TestLoader().loadTestsFromTestCase(ImportTest))
    tests.append(unittest.TestLoader().loadTestsFromTestCase(VerifyData))
    return unittest.TestSuite(tests)
