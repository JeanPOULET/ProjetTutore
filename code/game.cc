#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>
int main() {
	static constexpr gf::Vector2u ScreenSize(1024, 768);
	// initialization
	gf::Window window("K.G.B.", ScreenSize);
	gf::RenderWindow renderer(window);
	// game loop
	gf::Clock clock;
	renderer.clear(gf::Color::White);

	// controls

	gf::ActionContainer actions;

	gf::Action closeWindowAction("Close window");
	closeWindowAction.addCloseControl();
	closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
	closeWindowAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Back);
	actions.addAction(closeWindowAction);

	gf::Action leftAction("Left");
	leftAction.addScancodeKeyControl(gf::Scancode::A);
	leftAction.addScancodeKeyControl(gf::Scancode::Left);
	leftAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::RightX, gf::GamepadAxisDirection::Negative);
	leftAction.setContinuous();
	actions.addAction(leftAction);

	gf::Action rightAction("Right");
	rightAction.addScancodeKeyControl(gf::Scancode::D);
	rightAction.addScancodeKeyControl(gf::Scancode::Right);
	rightAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::RightX, gf::GamepadAxisDirection::Positive);
	rightAction.setContinuous();
	actions.addAction(rightAction);

	gf::Action upAction("Up");
	upAction.addScancodeKeyControl(gf::Scancode::W);
	upAction.addScancodeKeyControl(gf::Scancode::Up);
	upAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
	upAction.setContinuous();
	actions.addAction(upAction);

	gf::Action downAction("Down");
	downAction.addScancodeKeyControl(gf::Scancode::S);
	downAction.addScancodeKeyControl(gf::Scancode::Down);
	downAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
	downAction.setContinuous();
	actions.addAction(downAction);

	gf::Action swapAction("Swap");
	swapAction.addScancodeKeyControl(gf::Scancode::Tab);
	swapAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::X);
	actions.addAction(swapAction);

	gf::Action mergeAction("Merge");
	mergeAction.addScancodeKeyControl(gf::Scancode::Space);
	mergeAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::LeftBumper);
	actions.addAction(mergeAction);

	while (window.isOpen()) {
		// 1. input
		gf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case gf::EventType::Closed:
				window.close();
				break;
			case gf::EventType::KeyPressed:
				break;
			default:
				break;
			}
		}
		// 2. update
		float dt = clock.restart().asSeconds();
		// 3. draw
		renderer.clear();
		// draw everything
		renderer.display();
	}
	return 0;
}