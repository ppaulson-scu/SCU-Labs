from vacuum_world_classes_def import *  #imports VacuumAgent
import unittest                         #import python testing library

class TestProblem1(unittest.TestCase):

    def test_vacuum_world(self):
         #state=["Dirty", "Dirty"]
         #location = 0
         #seq = []
         a = VacuumAgent() # a vacuum cleaner agent
         self.assertEqual(a.run(["Dirty", "Dirty"], 0, []), ["Suck", "Right", "Suck"])  #Check to see if the resultant sequences
         self.assertEqual(a.run(["Dirty", "Dirty"], 1, []), ["Suck","Left", "Suck"])    #are the same as the expected ones

#If this is the top-level program, run unittest.main()            
if __name__ == '__main__':
   unittest.main()

  
