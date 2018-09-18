import unittest
from TestUtils import GetTestTextile
from TexGen.GridFile import ExportGridFile
from TexGen.Core import *

class GridFileTest(unittest.TestCase):
    def setUp(self):
        AddTextile('Textile', GetTestTextile())

    def tearDown(self):
        DeleteTextiles()

    def testGridFile(self):
        ExportGridFile('test.grd', 'Textile', (40, 40))

def GetTestSuite():
    suite = unittest.TestLoader().loadTestsFromTestCase(GridFileTest)
    return suite








