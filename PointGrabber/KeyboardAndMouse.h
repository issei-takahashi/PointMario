#pragma once

class KeyboardAndMouse
{
public:
	static void keyboard_callback (const pcl::visualization::KeyboardEvent& _evt, void* cookie);
	static void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie);
};