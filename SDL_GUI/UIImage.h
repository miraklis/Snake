#ifndef UIIMAGE_H
#define UIIMAGE_H

#include "UIMain.h"
#include "UIComponent.h"

namespace SDL_GUI {

	class UIImage : public UIComponent {
	public:
		UIImage(SDL_Renderer* renderer, std::string imgName, int x, int y);
		~UIImage();
		void Render() override;
	private:
		SDL_Texture* texture;
	};

}

#endif //UIIMAGE_H

