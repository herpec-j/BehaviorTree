NAME						= Test

PROJECT_DIRECTORY				= .

ID						= BehaviorTree::Test

ifeq						($(X64), 1)
ARCHITECTURE					= x64
ARCHITECTURE_OPTION				= -m64
else
ARCHITECTURE					= x86
ARCHITECTURE_OPTION				= -m32
endif

ifeq						($(DEBUG), 1)
CONFIGURATION					= Debug
else
CONFIGURATION					= Release
endif

BINARY_DIRECTORY				= $(PROJECT_DIRECTORY)/$(CONFIGURATION)/linux/$(ARCHITECTURE)

OBJECTS_DIRECTORY				= $(BINARY_DIRECTORY)/Objects
DEPENDENCIES_DIRECTORY				= $(BINARY_DIRECTORY)/Dependencies

EXECUTABLE_NAME					= $(BINARY_DIRECTORY)/$(NAME)

SOURCES_DIRECTORY				= $(PROJECT_DIRECTORY)/Sources
INCLUDES_DIRECTORY				= $(PROJECT_DIRECTORY)/Includes/BehaviorTree/
LIBRARIES_DIRECTORY				= $(PROJECT_DIRECTORY)/ThirdParties

SOURCES_EXTENSION				= cpp
OBJECTS_EXTENSION				= opp
DEPENDENCIES_EXTENSION				= dpp

SOURCES 					= $(shell find $(SOURCES_DIRECTORY) -name "*.$(SOURCES_EXTENSION)")
OBJECTS						= $(patsubst $(SOURCES_DIRECTORY)/%, $(OBJECTS_DIRECTORY)/%, $(SOURCES:.$(SOURCES_EXTENSION)=.$(OBJECTS_EXTENSION)))
DEPENDENCIES					= $(patsubst $(OBJECTS_DIRECTORY)/%, $(DEPENDENCIES_DIRECTORY)/%, $(OBJECTS:.$(OBJECTS_EXTENSION)=.$(DEPENDENCIES_EXTENSION)))
LIBRARIES					= $(shell ls $(LIBRARIES_DIRECTORY))
LIBRARIES					:= $(foreach LIBRARY, $(LIBRARIES),-I $(LIBRARIES_DIRECTORY)/$(LIBRARY)/include)

VERBOSE						= 0

CXX 						= g++
CXX_0						= @$(CXX)
CXX_1						= $(CXX)
COMPILER					= $(CXX_$(VERBOSE))

CXXFLAGS					= -W -Wall -Wextra -pipe
CXXFLAGS					+= -I $(INCLUDES_DIRECTORY) -I $(SOURCES_DIRECTORY)
CXXFLAGS					+= $(LIBRARIES)
CXXFLAGS					+= -std=c++11
CXXFLAGS					+= $(ARCHITECTURE_OPTION)

OPTIMIZATION_FLAGS				= -O3 -march=native
DEBUG_FLAGS					= -g3 -ggdb3 -D DEBUG
PROFILING_FLAGS					= $(DEBUG_FLAGS) -pg

ifeq						($(DEBUG), 1)
CXXFLAGS					+= $(DEBUG_FLAGS)
else ifeq					($(PROFILING), 1)
CXXFLAGS					+= $(PROFILING_FLAGS)
else
CXXFLAGS					+= $(OPTIMIZATION_FLAGS)
endif

LDFLAGS						+= -lm

RM						= rm -rf

MKDIR						= mkdir -p

MAKE						= make --no-print-directory -C

ifeq						($(VERBOSE), 1)
PRINTF						= true
else
PRINTF						= printf
endif

OK_MESSAGE					= '[\033[32mOK\033[0m]'

all:						$(EXECUTABLE_NAME)

$(EXECUTABLE_NAME):				$(OBJECTS)
						@$(MKDIR) $(BINARY_DIRECTORY)
						$(COMPILER) $(OBJECTS) $(CXXFLAGS) $(LDFLAGS) -o $(EXECUTABLE_NAME)
						@$(PRINTF) $(OK_MESSAGE)'[\033[36m$(ID)::$(BINARY_DIRECTORY)/$(NAME)\033[0m] linked\n'

-include					$(DEPENDENCIES)

clean:
						@$(RM) $(OBJECTS_DIRECTORY)
						@$(PRINTF) $(OK_MESSAGE)'[\033[31m$(ID)::Objects\033[0m] cleaned\n'
						@$(RM) $(DEPENDENCIES_DIRECTORY)
						@$(PRINTF) $(OK_MESSAGE)'[\033[31m$(ID)::Dependencies\033[0m] cleaned\n'

fclean: 					clean
						@$(RM) $(EXECUTABLE_NAME)
						@$(PRINTF) $(OK_MESSAGE)'[\033[31m$(ID)::Executable\033[0m] cleaned\n'

re:						fclean all

run:						all
						@$(EXECUTABLE_NAME)

.PHONY:						clean fclean re all run

$(OBJECTS_DIRECTORY)/%.$(OBJECTS_EXTENSION):	$(SOURCES_DIRECTORY)/%.$(SOURCES_EXTENSION)
						@$(eval DIRECTORY := $(shell dirname $* | sed 's/$\\.//'))
						@$(MKDIR) $(OBJECTS_DIRECTORY)/$(DIRECTORY)
						@$(MKDIR) $(DEPENDENCIES_DIRECTORY)/$(DIRECTORY)
						$(eval DEPENDENCY := $(patsubst $(OBJECTS_DIRECTORY)/%, $(DEPENDENCIES_DIRECTORY)/%, $(@:.$(OBJECTS_EXTENSION)=.$(DEPENDENCIES_EXTENSION))))
						$(COMPILER) -MD -MF $(DEPENDENCY) -c -o $@ $< $(CXXFLAGS)
						@$(PRINTF) $(OK_MESSAGE)'[\033[33m$(ID)::$<\033[0m] object builded (\033[34m$(ID)::$(OBJECTS_DIRECTORY)/$*.$(OBJECTS_EXTENSION)\033[0m)\n'
						@$(PRINTF) $(OK_MESSAGE)'[\033[33m$(ID)::$<\033[0m] dependency builded (\033[35m$(ID)::$(DEPENDENCIES_DIRECTORY)/$*.$(DEPENDENCIES_EXTENSION)\033[0m)\n'
