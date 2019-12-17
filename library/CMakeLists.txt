add_library(Box2D
	Box2D/Collision/Shapes/b2ChainShape.cpp
	Box2D/Collision/Shapes/b2CircleShape.cpp
	Box2D/Collision/Shapes/b2EdgeShape.cpp
	Box2D/Collision/Shapes/b2PolygonShape.cpp
	Box2D/Collision/b2BroadPhase.cpp
	Box2D/Collision/b2CollideCircle.cpp
	Box2D/Collision/b2CollideEdge.cpp
	Box2D/Collision/b2CollidePolygon.cpp
	Box2D/Collision/b2Collision.cpp
	Box2D/Collision/b2Distance.cpp
	Box2D/Collision/b2DynamicTree.cpp
	Box2D/Collision/b2TimeOfImpact.cpp
)

add_library(Box2D
	Box2D/Common/b2BlockAllocator.cpp
	Box2D/Common/b2Draw.cpp
	Box2D/Common/b2Math.cpp
	Box2D/Common/b2Settings.cpp
	Box2D/Common/b2StackAllocator.cpp
	Box2D/Common/b2Timer.cpp
)

add_library(Box2D
	Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp
	Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp
	Box2D/Dynamics/Contacts/b2CircleContact.cpp
	Box2D/Dynamics/Contacts/b2Contact.cpp
	Box2D/Dynamics/Contacts/b2ContactSolver.cpp
	Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp
	Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp
	Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp
	Box2D/Dynamics/Contacts/b2PolygonContact.cpp
	Box2D/Dynamics/Joints/b2DistanceJoint.cpp
	Box2D/Dynamics/Joints/b2FrictionJoint.cpp
	Box2D/Dynamics/Joints/b2GearJoint.cpp
	Box2D/Dynamics/Joints/b2Joint.cpp
	Box2D/Dynamics/Joints/b2MotorJoint.cpp
	Box2D/Dynamics/Joints/b2MouseJoint.cpp
	Box2D/Dynamics/Joints/b2PrismaticJoint.cpp
	Box2D/Dynamics/Joints/b2PulleyJoint.cpp
	Box2D/Dynamics/Joints/b2RevoluteJoint.cpp
	Box2D/Dynamics/Joints/b2RopeJoint.cpp
	Box2D/Dynamics/Joints/b2WeldJoint.cpp
	Box2D/Dynamics/Joints/b2WheelJoint.cpp
	Box2D/Dynamics/b2Body.cpp
	Box2D/Dynamics/b2ContactManager.cpp
	Box2D/Dynamics/b2Fixture.cpp
	Box2D/Dynamics/b2Island.cpp
	Box2D/Dynamics/b2World.cpp
	Box2D/Dynamics/b2WorldCallbacks.cpp
)

add_library(Box2D Box2D/Rope/b2Rope.cpp)

target_include_directories(Box2D
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/../include>
)

if (GF_BUILD_WITH_OPENGL3)
  target_compile_definitions(Box2D
    PRIVATE GF_OPENGL3
  )
endif()

if (GF_DEBUG)
  target_compile_definitions(Box2D
    PRIVATE GF_DEBUG
  )

endif()

if (GF_SHARED)
  target_compile_definitions(Box2D
    PUBLIC GF_SHARED
  )

endif()

target_compile_definitions(Box2D
  PRIVATE
    GF_EXPORT_SYMBOLS
    ZLIB_CONST
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(Box2D
    gcc_s
    gcc
  )
endif()
