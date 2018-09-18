import unittest
from math import pi
from TestUtils import GetTestWeave
from TexGen.Abaqus import TextileDeformerAbaqus
from TexGen.Core import *

class AbaqusTest(unittest.TestCase):
    def setUp(self):
        AddTextile('Textile', GetTestWeave())

    def tearDown(self):
        DeleteTextiles()

    def testAbaqus(self):
        shear = CLinearTransformation()
        shear.AddSheardYbydX(0.1)
        shear.AddSheardXbydY(0.1)

        tension = CLinearTransformation()
        tension.Scale(1.1)

        rotation = CLinearTransformation()
        rotation.Rotate(WXYZ(pi/8, 0, 0))

        deformer = TextileDeformerAbaqus()
        deformer.AddDeformationStep(shear)
        deformer.AddDeformationStep(shear*tension)
        deformer.AddDeformationStep(rotation)
        deformer.SetFortranProgram('Data/LinearElasticUMAT.for')
        deformer.AddMaterial("ISO_WARP", [1.0, 0.0])
        deformer.AddMaterial("ISO_WEFT", [2.0, 0.0])
        deformer.AssignMaterial('ISO_WARP', [0, 1])
        deformer.AssignMaterial('ISO_WEFT', [2, 3])
        deformer.SetSimulationFilesPrefix('AbaqusTest')

        textile = GetTextile()
        deformer.DeformTextile(textile)


def GetTestSuite():
    suite = unittest.TestLoader().loadTestsFromTestCase(AbaqusTest)
    return suite
