CCV=g++
CCVSTD= -std=c++98
OPTM=-O3
CCVFLAGS= -Wall -Werror -Wpedantic $(CCVSTD) $(OPTM)

make: turbo-trello.cc
	$(CCV) $(CCVFLAGS) -g -o turbo-trello turbo-trello.cc
turbo-trello: turbo-trello.cc
	$(CCV) $(CCVFLAGS) -o turbo-trello turbo-trello.cc
build: turbo-trello.cc
	$(CCV) $(CCVFLAGS) -o turbo-trello turbo-trello.cc
debug:
	$(CCV) $(CCVFLAGS) -g -o turbo-trello turbo-trello.cc
run:
	$(CCV) $(CCVFLAGS) -o turbo-trello turbo-trello.cc
	./turbo-trello
