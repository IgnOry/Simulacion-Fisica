#include <tuple>
#include "Resources.h"
#include "sdl_includes.h"

vector<Resources::FontInfo> Resources::fonts_{
//		{FIPPS20, "resources/fonts/Fipps-Regular.otf",20}
};

vector<Resources::ImageInfo> Resources::images_{

	//Enum, file path, frame width, frame height, columns, rows, frame total//
};

vector<Resources::TextMsgInfo> Resources::messages_{
	//{ HelloWorld, "Hello World", { COLOR(0xaaffffff) }, ARIAL16 }
};

vector<Resources::MusicInfo> Resources::musics_{
	{Laser , "resources/sound/Laser.mp3"},

	/*
	Echar gasolina
	Nave destruida
	Estrella destruida
	Planeta destruido

	Da�o nave
	Da�o carga

	Victoria (FF Fanfare)
	Derrota https://www.youtube.com/watch?v=mF31DPU-Rrs
	*/
};

vector<Resources::SoundInfo> Resources::sounds_{
};


vector<Resources::SpecialInfo> Resources::specialImages_{
};