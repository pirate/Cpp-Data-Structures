# -*- coding: utf-8 -*-
# Nick Sweeting ® 2013
# TicTacToe AI for C++ Data Stuctures -- Sunset HS

# A | B | C
# D | E | F
# G | H | I

a=b=c=d=e=f=g=h=i=0

board = [ a, b, c, \
		  d, e, f, \
		  g, h, i  ]

solutions = [[a,b,c],[d,e,f],[g,h,i],[a,d,g],[b,e,h],[c,f,i],[a,e,i],[c,e,g]]


def solution_compare(solution):
	if solution[0] == solution[1] == solution[2] == 1:
		return 1
	elif solution[0] == solution[1] == solution[2] == 2:
		return 2
	elif solution[0] == solution[1] == solution[2] == 0:
		return 0
	# else:
	# 	x=o=e=0 # e is for empty
	# 	for square in solution:
	# 		if square == 1: 
	# 			x += 1;
	# 		elif square == 2:
	# 			o += 1;
	# 		else:
	# 			e += 1;
	# 	return x+o+e

def isOver(board, solutions):
	for solution in solutions:
		result = solution_compare(solution)
		if result: 
			print "Won: "+str(result)
			return True
	if all(board): 
		print "Tie"
		return True
	return False

def printBoard(board):
	print ""

def solutionrank(solutions, board):
	