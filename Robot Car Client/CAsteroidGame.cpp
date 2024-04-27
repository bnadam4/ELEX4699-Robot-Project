#include "stdafx.h"
#include "CAsteroidGame.h"

#define S1_BUTTON 33
#define S2_BUTTON 32
#define XCH 2
#define YCH 26

CAsteroidGame::CAsteroidGame()
{
	controller.init_com(3);
	board = cv::Size(600, 600);
	_canvas = cv::Mat::zeros(board, CV_8UC3);
	score = 0;
	start_tic = cv::getTickCount(); // Get number of ticks since event (such as computer on)
	first_push = 0;
}

void CAsteroidGame::update()
{
	int firing = 0;
	int reseting = 0;

	// Implement hold and mash fire
	controller.get_data(DIGITAL, S1_BUTTON, firing);

	if (!firing)
	{
		if (!first_push)
		{
			std::cout << "Missile fired" << std::endl;
			first_push = 1;
			CMissile Missile(Ship.get_pos());
			Missile_list.push_back(Missile);
			start_tic = cv::getTickCount();
		}
        else if (((cv::getTickCount() - start_tic) / cv::getTickFrequency()) > 0.2)
		{
			std::cout << "Missile fired" << std::endl;
			CMissile Missile(Ship.get_pos());
			Missile_list.push_back(Missile);
			start_tic = cv::getTickCount();
		}
	}
	else
	{
		first_push = 0;
	}

	controller.get_data(DIGITAL, S2_BUTTON, reseting);

	while (!reseting)
	{
		if (((cv::getTickCount() - start_tic) / cv::getTickFrequency()) > 0.01)
		{
			score = 0;
			Ship.set_pos(cv::Point(board.width / 2, board.height / 2));
			Asteroid_list.clear();
			Missile_list.clear();
			Ship.set_lives(10);
			start_tic = cv::getTickCount();
		}
		controller.get_data(DIGITAL, S2_BUTTON, reseting);
	}

	cv::Point2f NewShipVel;
	cv::Point2f NewShipPos;
	cv::Point2f OldShipPos;

	NewShipVel.x = (controller.get_analog(XCH) - 50.0)/2.0;
	NewShipVel.y = (-(controller.get_analog(YCH) - 50.0))/2.0;

	OldShipPos = Ship.get_pos();
	NewShipPos = OldShipPos;

	if ((abs(NewShipVel.x) > 1.5) || (abs(NewShipVel.y) > 1.5))
	{
		if (Ship.collide_wall(board))
		{
			if (OldShipPos.y > board.height)
			{
				NewShipPos.y = 0;
			}
			if (OldShipPos.y < 0)
			{
				NewShipPos.y = board.height;
			}
			if (OldShipPos.x > board.width)
			{
				NewShipPos.x = 0;
			}
			if (OldShipPos.x < 0)
			{
				NewShipPos.x = board.width;
			}
		}

		if (!Ship.collide_wall(board))
		{
			NewShipPos.x = OldShipPos.x + NewShipVel.x;
			NewShipPos.y = OldShipPos.y + NewShipVel.y;
		}
	}

	Ship.set_pos(NewShipPos);

	// Move the asteroids
	int A_index = 0;
	for (A_index; A_index < Asteroid_list.size(); A_index++)
	{
		Asteroid_list[A_index].move();
	}

	// Move the Missiles
	int M_index = 0;
	for (M_index; M_index < Missile_list.size(); M_index++)
	{
		Missile_list[M_index].move();
	}

	// Detect collisions between the player and asteroids
	A_index = 0;

	for (A_index; A_index < Asteroid_list.size(); A_index++)
	{
		if (Asteroid_list[A_index].collide(Ship))
		{
			Asteroid_list[A_index].hit();
			Ship.hit();
		}
	}

	// Detect collisions between missiles and asteroids
	A_index = 0;
	M_index = 0;
	for (M_index; M_index < Missile_list.size(); M_index++)
	{
		for (A_index; A_index < Asteroid_list.size(); A_index++)
		{
			if (Missile_list[M_index].collide(Asteroid_list[A_index]))
			{
				Missile_list[M_index].hit();
				Asteroid_list[A_index].hit();
				score += 10;
			}
		}
	}

	// Detect Missile collisions with the side of the display
	M_index = 0;
	for (M_index; M_index < Missile_list.size(); M_index++)
	{
		if (Missile_list[M_index].collide_wall(board))
		{
			Missile_list[M_index].hit();
		}
	}

	// Detect Asteroid collisions with the side of the display
	A_index = 0;
	for (A_index; A_index < Asteroid_list.size(); A_index++)
	{
		if (Asteroid_list[A_index].collide_wall(board))
		{
			Asteroid_list[A_index].hit();
		}
	}

	// Remove 0 life missiles from the game
	M_index = 0;
	for (M_index; M_index < Missile_list.size(); M_index++)
	{
		if (Missile_list[M_index].get_lives() <= 0)
		{
			Missile_list.erase(Missile_list.begin() + M_index);
			M_index--;
		}
	}

	// Remove 0 life asteroids from the game
	A_index = 0;
	for (A_index; A_index < Asteroid_list.size(); A_index++)
	{
		if (Asteroid_list[A_index].get_lives() <= 0)
		{
			Asteroid_list.erase(Asteroid_list.begin() + A_index);
			A_index--;
		}
	}
}

char CAsteroidGame::draw()
{
	_canvas.setTo(cv::Scalar(0, 0, 0));

	Ship.draw(_canvas);

	// Draw all of the Asteroids
	int A_index = 0;
	for (A_index; A_index < Asteroid_list.size(); A_index++)
	{
		Asteroid_list[A_index].draw(_canvas);
	}

	// Draw all of the Missiles
	int M_index = 0;
	for (M_index; M_index < Missile_list.size(); M_index++)
	{
		Missile_list[M_index].draw(_canvas);
	}

	// Create menu window
	cv::Point gui_position = cv::Point(20, 20);
	cvui::window(_canvas, gui_position.x, gui_position.y, 150, 120, "Asteroids Game");
	gui_position += cv::Point(10, 30);
	std::string score_text = "Player Score: ";
	score_text += std::to_string(score);
	cvui::text(_canvas, gui_position.x, gui_position.y, score_text);
	gui_position += cv::Point(0, 30);
	std::string Ship_lives = "Ship lives: ";
	Ship_lives += std::to_string(Ship.get_lives());
	cvui::text(_canvas, gui_position.x, gui_position.y, Ship_lives);
	gui_position += cv::Point(0, 30);
	std::string Missile_text = "Missiles active: ";
	Missile_text += std::to_string(Missile_list.size());
	cvui::text(_canvas, gui_position.x, gui_position.y, Missile_text);

	if (Ship.get_lives() <= 0)
	{
		gui_position = cv::Point(250, 300);
		cvui::text(_canvas, gui_position.x, gui_position.y, "Game Over");
		gui_position += cv::Point(0, 30);
		cvui::text(_canvas, gui_position.x, gui_position.y, "Press Reset to Play again");
		Ship.set_pos(cv::Point(300, 200));
		Ship.set_color(cv::Scalar(0, 0, 200));
	}
	else
	{
		Ship.set_color(cv::Scalar(200, 0, 0));
	}

	// Show image and delay (all display image to update)
	cvui::update();
	cv::imshow(CANV_NAME, _canvas);

	if (cv::waitKey(1) == 'q')
	{
		return 'q';
	}

	return 'X';
}

void CAsteroidGame::run()
{
	char key = 'X';
	double start_time = cv::getTickCount() / cv::getTickFrequency();
	float spawntime = ((float)rand() / (float)RAND_MAX) * 5.0 + 1.0;
	double current_time;

	while (key != 'q')
	{
		current_time = cv::getTickCount() / cv::getTickFrequency();

		if ((current_time - start_time) > spawntime)
		{
			CAsteroid astro;
			Asteroid_list.push_back(astro);
			start_time = cv::getTickCount() / cv::getTickFrequency();
			spawntime = ((float)rand() / (float)RAND_MAX) * 5.0 + 1.0;
		}

		update();

		key = draw();
	}

	cv::destroyWindow(CANV_NAME);
}