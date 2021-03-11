#ifndef UIMENU_H
#define UIMENU_H

#include "UIMain.h"
#include "UIPanel.h"

namespace SDL_GUI {

	class UIMenu : public UIPanel, public std::enable_shared_from_this<UIMenu> {
	public:
		UIMenu(SDL_Renderer* renderer, std::string name, int x, int y, size_t w, size_t h, 
			   std::string fontName = sFonts::TTF_TIMES, size_t fontSize = 16,
			   HorizontalAlign hTextAlign = HorizontalAlign::Center, VerticalAlign vTextAlign = VerticalAlign::Middle, VerticalAlign vAlign=VerticalAlign::Top,
			   SDL_Color bgColor = { 0,0,0,0 }, SDL_Color fgColor = { 255,255,255,255 }, SDL_Color selBGColor = { 255,255,255,255 }, SDL_Color selFGColor = { 0,0,0,0 },
			   std::shared_ptr<UIMenu> parent=nullptr, std::function<bool(std::string)> callback=nullptr);
		~UIMenu();
		std::shared_ptr<UIMenu> CreateSubMenu(std::string name);
		void HandleEvents();
		void AddItem(std::string name, std::string caption);
		void SelectItem(size_t index);
		std::shared_ptr<UIMenu> GetParent();
	private:
		std::function<bool(std::string)> callback;
		std::shared_ptr<UIMenu> parent;
		size_t selectedItem;
		size_t upLimit;
		SDL_Color selFGColor;
		SDL_Color selBGColor;
	};

}

#endif //UIMENU_H