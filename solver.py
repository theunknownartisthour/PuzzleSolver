
import sys
import time
import json
import numpy as np

DIRECTIONS = [(-1, 0),
              ( 0,-1),
              ( 0, 1),
              ( 1, 0)]


class Solver(object):

    def __init__(self, state):
        self.state = state
        self.solution = None
        self.visited = set()
        self._steps = None

    def _solve(self):
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

    def solve(self):
        t0 = time.time()
        self._solve()
        print "\n\nOptimal Solution Found\n{} nodes visited\ntime = {} seconds\n\n".format(
            len(self.visited), time.time() - t0)
        return self.get_steps()

    def _get_steps(self):
        state = self.solution
        while state is not None:
            yield state
            state = state.parent

    def get_steps(self):
        if self._steps is None:
            steps = reversed(list(self._get_steps()))
            self._steps = {i: e.to_list() for i, e in enumerate(steps)}
        return self._steps


    def write_to_json(self, path):
        f = open(path, 'w')
        f.write("solution = ")
        obj = self.get_steps()
        d = {}
        for i in obj:
            lst = []
            x = obj[i]
            for j in x:
                lst.append([str(k) for k in j])
            d[i] = lst
        json.dump(d, f)
        f.write(";")
        f.close()
        


class State(tuple):
    '''
    Klotski board state. 
    An immutable tuple of tuples representing a unique board state
    '''
    
    def __init__(self, *rows):
        rows = [tuple(r) for r in rows]
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
        return self[4][1] == self[4][2] == "J"
    
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


def map_block_locations(state):
    locs = {}
    for i in xrange(len(state)):
        for j in xrange(len(state[0])):
            blockId = state[i][j]
            if not blockId:
                continue
            if blockId not in locs:
                locs[blockId] = set()
            locs[blockId].add((i, j))
    return locs


if __name__ == '__main__':
    f = open('puzzle.json', 'r')
    js = json.load(f)['puzzle']

    puzzle = State(js)
    t0 = time.time()
    solver = Solver(puzzle)

    solution = solver.solve()
    solver.write_to_json('solution.jsonp')
    for i, step in solution.iteritems():
        print i, State(step)


