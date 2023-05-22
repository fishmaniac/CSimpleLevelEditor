#include "common.h"
#include <stdbool.h>



#ifndef APP_H
#define APP_H

typedef struct {
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Point mousePoint;
	TTF_Font *font;
	int x, y, mouseX, mouseY, windowWidth, windowHeight, camX, camY, initialWindowWidth, initialWindowHeight, clickType, levelWidth, levelHeight, distanceTraveled;
	bool up, down, left, right, leftClick, rightClick, menu;
} App_s;

typedef struct {
	SDL_Rect rect;
	SDL_Texture *texture;
	SDL_Surface *surface;
	char text[MAX_TEXT], prefix[MAX_TEXT];
	int textWidth, textHeight, textLength, centerX, centerY, arrLength;
	bool focused, colliding, leftClick, rightClick;
} Button_s;

typedef struct {
	int type;
	SDL_Rect rect;
	bool colliding;
} Tile_s;

typedef struct {
	unsigned width;
	unsigned height;
	Tile_s *tile[];
} Grid_s;

#endif

void appLoop(App_s *app, Grid_s *grid);

void initMenu(App_s *app);
void handleTextInput(Button_s *inputBox, SDL_Event event);
Grid_s *initMap(unsigned width, unsigned height);
void renderMap(App_s *app, Grid_s *grid);
void checkCollision(App_s *app, Button_s *button);
bool checkCollisionMap(App_s *app, Tile_s *tile);
void createButton(App_s *app, Button_s button[]);
void doInput(App_s *app, Button_s button[]);
void doKeyUp(App_s *app, SDL_KeyboardEvent *event);
void doKeyDown(App_s *app, Button_s button[], SDL_KeyboardEvent *event);
void renderPrepare(App_s *app);
void initWindow(App_s *app);
void cleanupWindow(App_s *app);
void resizeRectangle(Button_s button[], App_s *app);
void updateCamera(App_s *app);

#ifdef _WIN32
#ifdef _WIN64
#define SDL_MAIN_HANDLED

int WinMain(int argc, char* argv[]) {
    SDL_SetMainReady();
    return main(argc, argv);
}
#endif
#endif

int main(int argc, char* argv[]) {
	App_s app;
	Grid_s *grid;

	app.menu = true;
	app.windowWidth = SCREEN_WIDTH;
	app.windowHeight = SCREEN_HEIGHT;
	app.initialWindowWidth = SCREEN_WIDTH;
	app.initialWindowHeight = SCREEN_HEIGHT;
	app.levelWidth = MAP_WIDTH;
	app.levelHeight = MAP_HEIGHT;
	app.left = false;
	app.right = false;
	app.up = false;
	app.down = false;
	app.x = 0;
	app.y = 0;
/* 	printf("x: %d\ty: %d\n", app.x, app.y); */

	initWindow(&app);
	initMenu(&app);
	
	printf("WIDTH: %d\tHEIGHT: %d\n", app.levelWidth, app.levelHeight);
	grid = initMap(app.levelWidth, app.levelHeight);
	
	// app.camX = (SCREEN_WIDTH / 2);
	// app.camY = (SCREEN_HEIGHT / 2);

	appLoop(&app, grid);
	

	printf("Hello, world!");
	free(grid);
	cleanupWindow(&app);
	
	return 0;
}

void appLoop(App_s *app, Grid_s *grid) {
	Button_s button[BUTTON_AMT_APP];
	button[0].rect.x = 0;
	button[0].rect.y = 0;
	button[0].rect.w = 64;
	button[0].rect.h = 32;
	button[0].focused = false;
	char widthText[] = "TILE";
	strcpy_s(button[0].text, sizeof(widthText), widthText);
	strcpy_s(button[0].prefix, 1, "");
	button[1].rect.x = 64;
	button[1].rect.y = 0;
	button[1].rect.w = 64;
	button[1].rect.h = 32;
	button[1].focused = false;
	char heightText[] = "PLATFORM";
	strcpy_s(button[1].text, sizeof(heightText), heightText);
	strcpy_s(button[1].prefix, 1, "");
	button[2].rect.x = 450;
	button[2].rect.y = app->windowHeight / 2;
	button[2].rect.w = 32;
	button[2].rect.h = 32;
	button[2].focused = false;
	char startText[] = "idk";
	strcpy_s(button[2].text, sizeof(startText), startText);
	strcpy_s(button[2].prefix, 1, "");
	
	app->clickType = 2;

	while (1) {
		Uint32 currentFrameTime = SDL_GetTicks();
		

		doInput(app, button);
		renderPrepare(app);


		renderMap(app, grid);
		createButton(app, button);
		checkCollision(app, &button[0]);
		checkCollision(app, &button[1]);
		checkCollision(app, &button[2]);
		
		SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
		
		SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 0);
		SDL_RenderFillRect(app->renderer, &button[0].rect);
		SDL_RenderCopy(app->renderer, button[0].texture, NULL, &button[0].rect);

		SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 0);
		SDL_RenderFillRect(app->renderer, &button[1].rect);
		SDL_RenderCopy(app->renderer, button[1].texture, NULL, &button[1].rect);

		SDL_SetRenderDrawColor(app->renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(app->renderer, &button[2].rect);
		SDL_RenderCopy(app->renderer, button[2].texture, NULL, &button[2].rect);
		
		//printf("W: %d H: %d\n", app->windowWidth, app->windowHeight);
		
		if (button[0].colliding && button[0].leftClick) {
			app->clickType = 2;
		}

		if (button[1].colliding && button[1].leftClick) {
			app->clickType = 3;
		}
		if (app->up) {
			app->y -= 1;
			app->up = false;
		}
		else if (!app->up) {
			app->y += 1;
		}
		if (app->down) {
			app->y += 1;
			app->down = false;
		}
		else if (!app->down) {
			app->y -= 1;
		}
		if (app->left) {
			app->x -= 1;
			app->left = false;
		}
		else if (!app->left) {
			app->x += 1;
		}
		if (app->right) {
			app->x += 1;
			app->right = false;
		}
		else if (!app->right) {
			app->x -= 1;
		}


		SDL_RenderPresent(app->renderer);
		SDL_RenderClear(app->renderer);


		Uint32 elapsedTime = SDL_GetTicks() - currentFrameTime;
		if (elapsedTime < FRAME_TIME) {
			SDL_Delay(FRAME_TIME - elapsedTime);
		}
	}
}

void initMenu(App_s *app) {
	TTF_Init();
	Button_s button[BUTTON_AMT];
	
	button[0].rect.w = 300;
	button[0].rect.h = 100;
	button[0].rect.x = 100;
	button[0].rect.y = 100;

	char widthPrefix[] = "Level Width: ";
	char widthText[] = "300";
	strcpy_s(button[0].text, sizeof(widthText), widthText);
	strcpy_s(button[0].prefix, sizeof(widthPrefix), widthPrefix);

	button[1].rect.w = 300;
	button[1].rect.h = 100;
	button[1].rect.x = app->windowWidth - (button[1].rect.w * 2);
	button[1].rect.y = button[1].rect.h;

	char heightPrefix[] = "Level Height: ";
	char heightText[] = "30";
	strcpy_s(button[1].text, sizeof(heightText), heightText);
	strcpy_s(button[1].prefix, sizeof(heightPrefix), heightPrefix);
	
	button[2].rect.w = 300;
	button[2].rect.h = 100;
	button[2].rect.x = app->windowHeight - (app->windowHeight / 10);
	button[2].rect.y = app->windowHeight / 2;

	button[2].focused = false;
	char startText[] = "START EDITOR";
	strcpy_s(button[2].text, sizeof(startText), startText);
	strcpy_s(button[2].prefix, 1, "");

	app->font = TTF_OpenFont("assets/font/slkscr.ttf", FONT_SIZE);
	if (app->font == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "UNABLE TO LOAD FONT %p\n", app->font);
 		exit(1);
	}
	else {
		while (app->menu) {
			Uint32 currentFrameTime = SDL_GetTicks();

			renderPrepare(app);

			createButton(app, button);
			checkCollision(app, &button[0]);
			checkCollision(app, &button[1]);
			checkCollision(app, &button[2]);


			SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(app->renderer, &button[0].rect);
			SDL_RenderCopy(app->renderer, button[0].texture, NULL, &button[0].rect);
			
			SDL_SetRenderDrawColor(app->renderer, 255, 0, 255, 255);
			SDL_RenderFillRect(app->renderer, &button[1].rect);
			SDL_RenderCopy(app->renderer, button[1].texture, NULL, &button[1].rect);
			
			SDL_SetRenderDrawColor(app->renderer, 0, 0, 255, 255);
			SDL_RenderFillRect(app->renderer, &button[2].rect);
			SDL_RenderCopy(app->renderer, button[2].texture, NULL, &button[2].rect);
			
			doInput(app, button);

			if (button[0].colliding && button[0].leftClick) {
				button[0].focused = true;
			}
			else if (!button[0].colliding) {
				button[0].focused = false;
			}
			if (button[1].colliding && button[1].leftClick) {
				button[1].focused = true;
			}
			else if (!button[1].colliding) {
				button[1].focused = false;
			}
			if (button[2].colliding && button[2].leftClick) {
				app->menu = false;
			}

			SDL_RenderPresent(app->renderer);
			SDL_RenderClear(app->renderer);

			Uint32 elapsedTime = SDL_GetTicks() - currentFrameTime;
			if (elapsedTime < FRAME_TIME) {
			SDL_Delay(FRAME_TIME - elapsedTime);
			}
		}
		app->levelWidth = atoi(button[0].text);
		app->levelHeight = atoi(button[1].text);
		printf("WIDTH: %d\tHEIGHT: %d\n", app->levelWidth, app->levelHeight);
		SDL_DestroyTexture(button[0].texture);
		SDL_FreeSurface(button[0].surface);
		SDL_DestroyTexture(button[1].texture);
		SDL_FreeSurface(button[1].surface);
		SDL_DestroyTexture(button[2].texture);
		SDL_FreeSurface(button[2].surface);

	}
}

void handleTextInput(Button_s button[], SDL_Event event) {
	for (int i = 0; i < BUTTON_AMT; i++) {
		if (button[i].focused) {
			strcat_s(button[i].text, sizeof(button->text), event.text.text);
		}
	}
}

Grid_s *initMap(unsigned width, unsigned height) {
	size_t bytes = sizeof(Grid_s);
	bytes += height * sizeof(Tile_s*);
	Grid_s *grid = malloc(bytes);
	grid->width = width;
	grid->height = height;
	grid->tile[0] = malloc(width * height * sizeof(Tile_s));

	for (unsigned ii = 1; ii < height; ii++) {
		grid->tile[ii] = grid->tile[0] + ii * width;
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			grid->tile[y][x].type = 0;
			grid->tile[y][x].rect.x = x * TILE_SIZE;
			grid->tile[y][x].rect.y = y * TILE_SIZE;
			grid->tile[y][x].rect.w = TILE_SIZE;
			grid->tile[y][x].rect.h = TILE_SIZE;
		}
	}
	
	return grid;
}

void renderMap(App_s *app, Grid_s *grid) {
	for (int y = 0; y < app->levelHeight; y++) {
		for (int x = 0; x < app->levelWidth; x++) {

			if (checkCollisionMap(app, &grid->tile[y][x]) && app->leftClick) {
				grid->tile[y][x].type = app->clickType;
				printf("TYPE: %d\ty: %d\tx: %d\tleftClick: %d\trightClick: %d\tclickType: %d\n", grid->tile[y][x].type, y, x, app->leftClick, app->rightClick, app->clickType);
			}
			if (checkCollisionMap(app, &grid->tile[y][x]) && app->rightClick) {
				grid->tile[y][x].type = 0;
				printf("TYPE: %d\ty: %d\tx: %d\tleftClick: %d\trightClick: %d\tclickType: %d\n", grid->tile[y][x].type, y, x, app->leftClick, app->rightClick, app->clickType);
			}

			SDL_Color color;
			if (grid->tile[y][x].type == 0) {
				color = COLOR_WHITE;
			} 
			else if (grid->tile[y][x].type == 2) {
				printf("RENDER MAP: x: %d\ty: %d\n", app->camX, app->camY);
				color = COLOR_BLACK;
			}
			else if (grid->tile[y][x].type == 3) {
				color = COLOR_GREEN;
			}


			SDL_SetRenderDrawColor(app->renderer, color.r, color.g, color.b, color.a);

			SDL_RenderFillRect(app->renderer, &grid->tile[y][x].rect);

			grid->tile[y][x].rect.x -= app->x;
			grid->tile[y][x].rect.y -= app->y;
		}
	}
}

void checkCollision(App_s *app, Button_s *button) {
	if (SDL_PointInRect(&app->mousePoint, &button->rect)) {
		button->colliding = true;
	}
	else {
		button->colliding = false;
	}
}

bool checkCollisionMap(App_s *app, Tile_s *tile) {
	if (SDL_PointInRect(&app->mousePoint, &tile->rect)) {
		tile->colliding = true;
		return true;
	}
	else {
		tile->colliding = false;
		return false;
	}
}

void createButton(App_s *app, Button_s button[]) {
	for (int i = 0; i < BUTTON_AMT; i++) {
		char prefixedText[2 * MAX_TEXT];
		strcpy(prefixedText, button[i].prefix);
		strcat(prefixedText, button[i].text);

		button[i].surface = TTF_RenderText_Solid(app->font, prefixedText, COLOR_BLACK);
		button[i].texture = SDL_CreateTextureFromSurface(app->renderer, button[i].surface);

		while (button[i].surface->w > button[i].rect.w) {
			button[i].surface->w /= 2;
			button[i].surface->h /= 2;
		}
		button[i].textWidth = button[i].surface->w;
		button[i].textHeight = button[i].surface->h;
		button[i].centerX = button[i].rect.x + (button[i].rect.w - button[i].textWidth) / 2;
		button[i].centerY = button[i].rect.y + (button[i].rect.h - button[i].textHeight) / 2;
	}
}

void renderPrepare(App_s *app) {
	SDL_SetRenderDrawColor(app->renderer, COLOR_BLUE.r, COLOR_BLUE.g, COLOR_BLUE.b, COLOR_BLUE.a);
	SDL_RenderClear(app->renderer);
}

void initWindow(App_s *app) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	//SDL_SetRenderDrawColor(app->renderer, COLOR_BLUE.r, COLOR_BLUE.g, COLOR_BLUE.b, COLOR_BLUE.a);
	app->window = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_PRESENTVSYNC);
	
	renderPrepare(app);
}

void cleanupWindow(App_s *app) {
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	TTF_CloseFont(app->font);
	TTF_Quit();
	SDL_Quit();
}

void doInput(App_s *app, Button_s button[]) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				doKeyDown(app, button, &event.key);
				break;
			case SDL_KEYUP:
				doKeyUp(app, &event.key);
				break;
			case SDL_TEXTINPUT:
				handleTextInput(button, event);
				break;
			case SDL_MOUSEMOTION:
				app->mousePoint.x = event.motion.x;
				app->mousePoint.y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					SDL_CaptureMouse(SDL_TRUE);
					app->leftClick = true;
					for (int i = 0; i < BUTTON_AMT; i++) {
						button[i].leftClick = true;
					}
					break;
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					SDL_CaptureMouse(SDL_TRUE);
					app->rightClick = true;
					for (int i = 0; i < BUTTON_AMT; i++) {
						button[i].rightClick = true;
					}
					break;
				}
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					SDL_CaptureMouse(SDL_FALSE);
					app->leftClick = false;
					for (int i = 0; i < BUTTON_AMT; i++) {
						button[i].leftClick = false;
					}
					break;
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					SDL_CaptureMouse(SDL_FALSE);
					app->rightClick = false;
					for (int i = 0; i < BUTTON_AMT; i++) {
						button[i].rightClick = false;
					}	
					break;
				}
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOW_MAXIMIZED || event.window.event == SDL_WINDOW_MINIMIZED) {
/* 					resizeRectangle(button, app); */
					app->windowWidth = event.window.data1;
					app->windowHeight = event.window.data2;
				}
				break;
			default:
				break;
		}
	}
}

void doKeyDown(App_s *app, Button_s button[], SDL_KeyboardEvent *event) {
	for (int i = 0; i < BUTTON_AMT; i++) {
		if (event->repeat == 0) {
			printf("BUTTON: leftClick: %d\trightClick: %d\tcolliding: %d\tfocused: %d\ttext: %d\n", button[i].leftClick, button[i].rightClick, button[i].colliding, button[i].focused, atoi(button[i].text));
			if (event->keysym.scancode == SDL_SCANCODE_BACKSPACE && button[i].focused) {
				printf("BUTTON: leftClick: %d\trightClick: %d\tcolliding: %d\tfocused: %d\ttext: %d\n", button[i].leftClick, button[i].rightClick, button[i].colliding, button[i].focused, atoi(button[i].text));
				int len = strlen(button[i].text);
				if (len > 0) {
					button[i].text[len - 1] = '\0';
					len--;
					if (len == 0) {
						strcpy(button[i].text, " ");
					}
				}
			}
			if (event->keysym.scancode == SDL_SCANCODE_W) {
				app->up = true;
			}
			if (event->keysym.scancode == SDL_SCANCODE_S) {
				app->down = true;
			}
			if (event->keysym.scancode == SDL_SCANCODE_A) {
				app->left = true;
			}
			if (event->keysym.scancode == SDL_SCANCODE_D) {
				app->right = true;
			}
		}
	}
}

void doKeyUp(App_s *app, SDL_KeyboardEvent *event) {
	if (event->repeat == 0) {
		if (event->keysym.scancode == SDL_SCANCODE_W) {
			app->up = false;
		}
		if (event->keysym.scancode == SDL_SCANCODE_S) {
			app->down = false;
		}
		if (event->keysym.scancode == SDL_SCANCODE_A) {
			app->left = false;
		}
		if (event->keysym.scancode == SDL_SCANCODE_D) {
			app->right = false;
		}
	}
}

void resizeRectangle(Button_s button[], App_s* app) {
	
	for (int i = 0; i < BUTTON_AMT; i++) {
		while (app->windowWidth > app->initialWindowWidth) {
			SDL_GetWindowSize(app->window, &app->windowWidth, &app->windowWidth);
			button[i].rect.w += 1;
		}
		while (app->windowWidth < app->initialWindowWidth) {
			SDL_GetWindowSize(app->window, &app->windowWidth, &app->windowWidth);
			button[i].rect.w -= 1;
		}
		while (app->windowHeight > app->initialWindowHeight) {
			SDL_GetWindowSize(app->window, &app->windowWidth, &app->windowWidth);
			button[i].rect.h += 1;
		}
		while (app->windowHeight < app->initialWindowHeight) {
			SDL_GetWindowSize(app->window, &app->windowWidth, &app->windowWidth);
			button[i].rect.h -= 1;
		}
	}
}

void updateCamera(App_s *app) {
	app->camX = app->x - (SCREEN_WIDTH / 2);
	app->camY = app->y - (SCREEN_HEIGHT / 2);
}
