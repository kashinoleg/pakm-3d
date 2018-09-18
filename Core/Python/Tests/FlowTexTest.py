import unittest
from TestUtils import GetTestTextile
from TexGen.FlowTex import ExportFlowTex
from TexGen.Core import *

class FlowTexTest(unittest.TestCase):
    def setUp(self):
        AddTextile('Textile', GetTestTextile())

    def tearDown(self):
        DeleteTextiles()

    def testFlowTex(self):
        ExportFlowTex('test.vox', 'Textile', (10, 10, 10))

def GetTestSuite():
    suite = unittest.TestLoader().loadTestsFromTestCase(FlowTexTest)
    return suite
