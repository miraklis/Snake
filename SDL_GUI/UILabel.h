#ifndef UILABEL_H
#define UILABEL_H

#include "UIMain.h"
#include "UIComponent.h"

namespace SDL_GUI {

	class UILabel : public UIComponent {
	public:
		UILabel(SDL_Renderer* renderer, std::string name, std::string caption, std::string fontName=sFonts::TTF_TIMES, size_t fontSize=16,
				int posX = 0, int posY = 0, size_t w = 0, size_t h = 0, bool autosize = true, HorizontalAlign hAlign=HorizontalAlign::Left, VerticalAlign vAlign=VerticalAlign::Top,
				SDL_Color bgColor = { 0,0,0,255 }, SDL_Color fgColor = { 255,255,255,255 });
		virtual ~UILabel();
		void Render() override;
		void SetSize(size_t w, size_t h) override;
		void SetText(std::string caption);
		void SetColor(const SDL_Color& bgColor, const SDL_Color& fgColor);
		void SetFont(std::string fntName, size_t fntSize);
		void AlignHorizontal(HorizontalAlign hAligh);
		void AlignVertical(VerticalAlign vAlign);
		void Align();
		void SetAutosize(bool autosize);
		Event<UILabel*> OnTextChanged;
	protected:
		bool autosize;
		std::string caption;
		SDL_Color fgColor;
		SDL_Color bgColor;
		TTF_Font* font;
		SDL_Texture* texture;
		SDL_Rect textRegion;
		int txtOffsetX;
		int txtOffsetY;
		HorizontalAlign hAlign;
		VerticalAlign vAlign;
	private:
		void updateText();
	};

}

#endif //UILABEL_H