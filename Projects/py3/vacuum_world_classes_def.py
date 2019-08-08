class VacuumAgent:

  #Returns 'True' if the state is the desired state, and 'False' if not.
  def goal_test(self, state):
              if state==["Clean", "Clean"]:
                            return True
              else:
                            return False

  #Determines the next action to take, based in room state and location.
  #If the room is dirty, suck; else swap to the other room.
  def action(self, state, location):
              if state[location] == "Dirty":
                            action = "Suck"
              elif location == 0:
                            action = "Right"
              else:
                            action = "Left"
              return action 
       
  #Updates the state of the room based in the action taken
  #If the room was Sucked, then change it to clean; else update location
  #to the new position.
  def update_state(self, state, action, location):
              if action=="Suck":
                     state[location] = "Clean"
                                      
              elif action=="Left":
                   location = 0
              else:
                   location = 1             
              return state, location
       

  #Runs the vacuum cleaner.
  def run(self, state, location, seq): 
       while True:
              if self.goal_test(state): #If the state is the desired state, return
                            return seq  #the sequence of actions taken to get there.
              action = self.action(state, location) #If not, take an action
              seq.append(action)                    #append the taken action to the sequence of actions taken
              state, location = self.update_state(state, action, location)  #Update the location of the cleaner and the state of the world.

       

       
  
                            
