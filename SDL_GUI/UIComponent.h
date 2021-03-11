#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include "UIMain.h"

namespace SDL_GUI {

	class UIComponent {
	public:
		virtual void Render() = 0;
		std::string GetName();
		void Show();
		void Hide();
		bool IsVisible();
		virtual void SetPos(int x, int y);
		void SetX(int x);
		void SetY(int y);
		virtual void SetSize(size_t w, size_t h);
		void SetWidth(size_t w);
		void SetHeight(size_t h);
		const SDL_Rect& GetRect();
		Event<UIComponent*> OnSizeChanged;
	protected:
		UIComponent(SDL_Renderer* renderer, std::string name,
					int x=0, int y=0, size_t w=0, size_t h=0, bool visible=false);
		virtual ~UIComponent();
		SDL_Renderer* renderer;
		std::string name;
		SDL_Rect rect;
		bool visible;
		virtual void SizeChanged(UIComponent* sender);
	};

}

#endif //UICOMPONENT_H