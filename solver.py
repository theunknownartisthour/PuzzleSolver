DIRECTIONS = [(-1, 0),
              ( 0,-1),
              ( 0, 1),
              ( 1, 0)]


class Solver(object):

    def __init__(self, state):
        self.state = state
        self.solution = None
        self.visited = set()
        
    def solve(self):
        queue = [self.state]
        while len(queue) and self.solution is None:
            current = queue.pop(0)
            if current not in self.visited:
                mirrored = current.mirror()
                self.visited.add(current)
                self.visited.add(mirrored)
                for successor in current.get_successors():                
                    if successor.is_solution():
                        self.solution = successor
                        return self.solution
                    queue.append(successor)
        return self.solution
        
    
    def get_steps(self):
        state = self.solution
        while state is not None:
            yield state
            state = state.parent
        
    def print_steps(self):
        steps = reversed(list(self.get_steps()))
        for i, e in enumerate(steps):
            print i, e


class State(tuple):
    '''
    Klotski board state. 
    An immutable tuple of tuples representing a unique board state
    '''
    
    def __init__(self, *rows):
        super(State, self).__init__(rows)
        self.rows = len(self)
        self.cols = len(self[0])
        self.locs = map_block_locations(self)
        self.parent = None

    def __repr__(self):
        return "State\n%s\n" % ("\n".join(map(str, self)),)
    
    def __eq__(self, other):
        return hash(self) == hash(other)
    
    def __hash__(self):
        return hash(frozenset([frozenset(self.locs[i]) for i in self.locs]))

    def mirror(self):
        # returns a horizontally mirrored version of this state
        return State([row[::-1] for row in self])
        
    def is_solution(self):
        # This is sorta hackish, not sure how to not require the hard coded indices
        return self[4][1] == self[4][2] == 1
    
    def get_eligible_directions(self, block_id):
        '''returns a list of eligible directions for a given block'''
        directions = DIRECTIONS[:]
        cells = self.locs[block_id]
        for cell in cells:
            for direction, neighbor in self.get_neighbors(cell).iteritems():
                if neighbor is not None:
                    value = self[neighbor[0]][neighbor[1]]
                    if value != block_id and value != 0 and direction in directions:
                        directions.remove(direction)
                elif direction in directions: 
                    directions.remove(direction)
        return directions

    def get_neighbors(self, cell):
        '''
        return potential directions to move from a position
        '''
        result = dict.fromkeys(DIRECTIONS)
        m,n = cell
        for x, y in DIRECTIONS:
            row, col = m + x, n + y
            if row >= 0 and row < self.rows and col >= 0 and col < self.cols:
                result[(x, y)] = (row, col)
        return result

    def get_movable_blocks(self):
        '''
        returns a list of (block id, direction) tuples of eligible moves
        '''
        return [(block_id, direction) for block_id in self.locs.keys()
                for direction in self.get_eligible_directions(block_id)]
    
    def get_successors(self):
        for block_id in self.locs:
            for direction in self.get_eligible_directions(block_id):
                yield move_block(self, block_id, direction)
                
    def to_list(self):
        return [list(i) for i in self]



def move_block(state, block_id, direction):
    '''
    return a new state, with the block 'block_id' moved one step in 'direction'
    Requires that the block is actually movable
    '''
    rows = state.to_list()
    for cell in state.locs[block_id]:
        rows[cell[0]][cell[1]] = 0
    for cell in state.locs[block_id]:
        new_loc = cell[0] + direction[0], cell[1] + direction[1]
        rows[new_loc[0]][new_loc[1]] = block_id
    result = State(map(tuple, rows))
    result.parent = state
    return result


def map_block_locations(board):
    locs = {}
    for i in xrange(len(board)):
        for j in xrange(len(board[0])):
            blockId = board[i][j]
            if not blockId:
                continue
            if blockId not in locs:
                locs[blockId] = set()
            locs[blockId].add((i, j))
    return locs


if __name__ == '__main__':
    import tests

    seven = State(tests.seven_steps)
    nine = State(tests.nine_steps)
    original = State(tests.original)

    print "Seven Steps\n"
    solver = Solver(seven)
    solver.solve()
    solver.print_steps()

    print "Nine Steps\n"
    solver = Solver(nine)
    solver.solve()
    solver.print_steps()

    print "116 Steps\n"
    solver = Solver(original)
    solver.solve()
    solver.print_steps()