cmake_minimum_required(VERSION 2.6)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

project(BehaviorTree)

if (MSVC)

else()
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if (COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	elseif (COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else ()
		message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif ()
endif ()

set(BehaviorTree_HDRS
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/BehaviorTree.hpp)

source_group(Includes FILES ${BehaviorTree_HDRS})

set(BehaviorTree_Details_HDRS
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/ActionNode.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/AlwaysFailureDecorator.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/AlwaysRunningDecorator.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/AlwaysSuccessDecorator.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/BooleanCondition.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/CompositeNode.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/ConcurrentSelector.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/ConditionNode.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/ConditionTest.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/CountLimitDecorator.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/DecimalCondition.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/DecoratorNode.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/FailureAfterDecorator.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/FailurePolicy.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/FunctionCondition.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/IntegerCondition.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/LeafNode.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/PrioritySelector.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/ProbabilitySelector.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/RandomSelector.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/RepeatDecorator.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/RootNode.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/RunningAfterDecorator.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/SequenceSelector.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/Status.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/SuccessAfterDecorator.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/SuccessPolicy.hpp)

source_group(Includes\\Details FILES ${BehaviorTree_Details_HDRS})	

set(BehaviorTree_Details_Private_HDRS
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/Private/DecimalConditionEnabler.hpp
	${PROJECT_SOURCE_DIR}/BehaviorTree/Includes/BehaviorTree/Details/Private/IntegerConditionEnabler.hpp)

source_group(Includes\\Details\\Private FILES ${BehaviorTree_Details_Private_HDRS})

set(HDRS
	${BehaviorTree_HDRS}
	${BehaviorTree_Details_HDRS}
	${BehaviorTree_Details_Private_HDRS})
	
set(SRCS
	${PROJECT_SOURCE_DIR}/BehaviorTree/Sources/Test.cpp)
	
source_group(Sources FILES ${SRCS})

include_directories("$(PROJECT_SOURCE_DIR)/BehaviorTree/Includes")

add_executable(BehaviorTreeTest ${SRCS} ${HDRS})

enable_testing()

add_test(BehaviorTreeUnitTests BehaviorTreeTest)

set_tests_properties(BehaviorTreeUnitTests
	PROPERTIES PASS_REGULAR_EXPRESSION "Working fine")