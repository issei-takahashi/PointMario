#include "KeyboardAndMouse.h"

// Simple callbacks.
void keyboard_callback (const pcl::visualization::KeyboardEvent& _evt, void* cookie)
{
	std::string* message = (std::string*)cookie;
	cout << (*message) << " :: ";
	if (_evt.getKeyCode()){
		switch(_evt.getKeyCode()){
		case 'r': // 録画開始
			{

				break;
			}
		}
	}
	else{
		cout << "the special key \'" << _evt.getKeySym() << "\' was";
	}
	if (_evt.keyDown()){
		cout << " pressed" << endl;
	}
	else{
		cout << " released" << endl;
	}
}

void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie)
{
	std::string* message = (std::string*) cookie;
	// 左クリック
	if (mouse_event.getType() == pcl::visualization::MouseEvent::MouseButtonPress &&
		mouse_event.getButton() == pcl::visualization::MouseEvent::LeftButton)
	{
		///cout << (*message) << " :: " << mouse_event.getX () << " , " << mouse_event.getY () << endl;
		
	}
}