#include "sfml_app.h"

#include <iostream>
#include <thread>

SfmlApp::SfmlApp(
	std::pair<unsigned int, unsigned int> window_size,
	std::pair<unsigned int, unsigned int> cell_size
)
	: window_({ window_size.first, window_size.second }, "My window")
	, cell_size_(cell_size)
{
}

SfmlApp::~SfmlApp()
{
}

std::string getMessageForActiveStatus(bool active)
{
	
	if (active)
	{
		return "Simulation is ACTIVE. Press <SPACE> to toggle the simulation. Press <ESC> to exit.";
	}

	return "Simulation is PAUSED. Press <SPACE> to toggle the simulation. Press <ESC> to exit.";
}

void SfmlApp::init()
{
	this->living_cell_color_ = sf::Color(40, 160, 20);
	this->dead_cell_color_ = sf::Color(25, 23, 21);

	if (font_.loadFromFile("../../data/OpenSans-Regular.ttf"))
	{
		gui_text_.setFont(font_);
		gui_text_.setString(getMessageForActiveStatus(false));
		gui_text_.move(10, 2);
		gui_text_.setCharacterSize(24);
		gui_text_.setOutlineColor(sf::Color::White);
	}
	else
	{
		// error...cannot render GUI
	}

	// Note: you can also override the window size after it is initialized.
	// This could be useful to allow changing the resolution and the world size after the initialization
	// but is not really a priority for this exercise.
	//this->window.setSize(sf::Vector2u(1200, 600));

	// Initialize the vertices
	unsigned max_width = static_cast<unsigned>(window_.getView().getSize().x);
	unsigned max_height = static_cast<unsigned>(window_.getView().getSize().y);

	// Store the world size for later use.
	world_size_.first = max_width / cell_size_.first - 1;
	world_size_.second = max_height / cell_size_.second - 1;

	// Initialize the world map with vertices.
	for (unsigned cell_y = 0; cell_y < max_height / cell_size_.first - 1; cell_y++)
	{
		for (unsigned cell_x = 0; cell_x < max_width / cell_size_.second - 1; cell_x++)
		{
			this->addVertexQuad(cell_x, cell_y, cell_size_.first, cell_size_.second);
		}
	}
	unsigned j, n;
	for (unsigned i = 0; i < world_size_.first; i++)
	{
		n = rand() % 40;
		
		for (size_t k = 0; k < n; k++)
		{
			j = rand() % world_size_.second;
			setCellColor(i, j, living_cell_color_);
		}
	}
	cell_vertices_new_gen = cell_vertices_;
}

void SfmlApp::run()
{
	unsigned time_elapsed_since_update = 0;
	bool simulation_active(false);
	// TODO: it would be nice to make this configurable in the future.
	unsigned duration_in_millis_between_updates = 1000;
	bool newCell = false;

	// run the program as long as the window is open
	while (window_.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window_.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				window_.close();
			}

			// quick close via the ESC key.
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window_.close();
					return;
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					// Toggle simulation
					simulation_active = !simulation_active;
					gui_text_.setString(getMessageForActiveStatus(simulation_active));
				}
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					unsigned view_width = static_cast<unsigned>(window_.getView().getSize().x);
					unsigned view_height = static_cast<unsigned>(window_.getView().getSize().y);
					unsigned win_width = static_cast<unsigned>(window_.getSize().x);
					unsigned win_height = static_cast<unsigned>(window_.getSize().y);

					unsigned clicked_cell_x = event.mouseButton.x * view_width / (cell_size_.first * win_width);
					unsigned clicked_cell_y = event.mouseButton.y * view_height / (cell_size_.second * win_height);

					// TODO: maybe update a world matrix?
					setCellColor(clicked_cell_x, clicked_cell_y, living_cell_color_);
					newCell = true;
					window_.clear(sf::Color::Black);
					render();
					window_.display();
					
					std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(1000));
					updateWorld();
					
				}
			}
		}

		if (time_elapsed_since_update > duration_in_millis_between_updates )
		{
			if (simulation_active)
			{
				updateWorld();
			}
			time_elapsed_since_update -= duration_in_millis_between_updates;
		}
		if (newCell)
		{
			time_elapsed_since_update = 0;
			newCell = false;
		}

		// clear the window with black color
		window_.clear(sf::Color::Black);

		render();

		// end the current frame
		window_.display();

		// don't consume too many cpu cycles
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(10));
		if (simulation_active)
		{
			time_elapsed_since_update += 10;
		}
	}
}

void SfmlApp::setCellColor(unsigned cell_x, unsigned cell_y, sf::Color color, bool newGen)
{
	switch (newGen)
	{
	case false:
	{
		auto index = (cell_y * this->world_size_.first + cell_x) * 4;
		if (index >= cell_vertices_.size())
		{
			return;
		}

		for (size_t i = 0; i < 4; i++)
		{
			cell_vertices_[index + i].color = color;
		}
		break;
	}
	case true:
	{
		auto index = (cell_y * this->world_size_.first + cell_x) * 4;
		if (index >= cell_vertices_new_gen.size())
		{
			return;
		}

		for (size_t i = 0; i < 4; i++)
		{
			cell_vertices_new_gen[index + i].color = color;
		}
		break;
	}
	default:
		break;
	}
}

void SfmlApp::addVertexQuad(unsigned cell_x, unsigned cell_y, unsigned width, unsigned height)
{
	sf::Vertex topLeft;
	sf::Vertex topRight;
	sf::Vertex bottomLeft;
	sf::Vertex bottomRight;

	float pixel_x = static_cast<float>(cell_x * width);
	float pixel_y = static_cast<float>(cell_y * height);

	topLeft.position = { pixel_x,			 pixel_y };
	topRight.position = { pixel_x + width,   pixel_y };
	bottomLeft.position = { pixel_x,           pixel_y + height };
	bottomRight.position = { pixel_x + width,   pixel_y + height };

	topLeft.color = dead_cell_color_;
	topRight.color = dead_cell_color_;
	bottomLeft.color = dead_cell_color_;
	bottomRight.color = dead_cell_color_;

	cell_vertices_.push_back(topLeft);
	cell_vertices_.push_back(bottomLeft);
	cell_vertices_.push_back(bottomRight);
	cell_vertices_.push_back(topRight);
}

void SfmlApp::render()
{
	// draw everything here...
	window_.draw(cell_vertices_.data(), cell_vertices_.size(), sf::Quads);

	// Add any GUI info.
	window_.draw(gui_text_);
}

int SfmlApp::isAlive(unsigned x, unsigned y)
{
	auto index = (y * this->world_size_.first + x) * 4;
	if (cell_vertices_[index].color == living_cell_color_)
	{
		return 1;
	}
	return 0;
}

int SfmlApp::numberOfNeighbours(unsigned x, unsigned y)
{
	int n = 0;
	
	if (x == 0)
	{
		if (y == 0)
		{
			n += isAlive(x,y + 1) + isAlive(x + 1,y) + isAlive(x + 1,y + 1);
		}
		else if (y == world_size_.second - 1)
		{
			n += isAlive(x, y - 1) + isAlive(x + 1, y) + isAlive(x + 1, y - 1);
		}
		else
		{
			n += isAlive(x, y - 1) + isAlive(x + 1, y - 1) + isAlive(x + 1, y) + isAlive(x + 1, y + 1) + isAlive(x, y + 1);
		}
	}
	else if (x == world_size_.first - 1)
	{
		if (y == 0)
		{
			n += isAlive(x, y + 1) + isAlive(x - 1, y) + isAlive(x - 1, y + 1);
		}
		else if (y == world_size_.second - 1)
		{
			n += isAlive(x, y - 1) + isAlive(x - 1, y) + isAlive(x - 1, y - 1);
		}
		else
		{
			n += isAlive(x, y - 1) + isAlive(x - 1, y - 1) + isAlive(x - 1, y) + isAlive(x - 1, y + 1) + isAlive(x, y + 1);
		}
	}
	else if (y == 0)
	{
		n += isAlive(x - 1, y) + isAlive(x - 1, y + 1) + isAlive(x, y + 1) + isAlive(x + 1, y + 1) + isAlive(x + 1, y);
	}
	else if (y == world_size_.second - 1)
	{
		n += isAlive(x - 1, y) + isAlive(x - 1, y - 1) + isAlive(x, y - 1) + isAlive(x + 1, y - 1) + isAlive(x + 1, y);
	}
	else
	{
		n += isAlive(x - 1, y - 1) + isAlive(x - 1, y) + isAlive(x - 1, y + 1) + isAlive(x, y - 1) + isAlive(x, y + 1) + isAlive(x + 1, y - 1) + isAlive(x + 1, y) + isAlive(x + 1, y + 1);
	}
	return n;
}

void SfmlApp::updateWorld()
{
	cell_vertices_new_gen = cell_vertices_;
	// TODO: feel free to add function arguments as deemed necessary.
	for (unsigned index1 = 0; index1 < world_size_.first; index1++)
	{
		for (unsigned index2 = 0; index2 < world_size_.second; index2++)
		{
			if (isAlive(index1, index2) == 0)
			{
				if (numberOfNeighbours(index1, index2) == 3)
				{
					setCellColor(index1, index2, living_cell_color_, true);
				}
			}
			else 
			{
				if (numberOfNeighbours(index1, index2) < 2 || numberOfNeighbours(index1, index2) > 3)
				{
					setCellColor(index1, index2, dead_cell_color_, true);
				}
			}
		}
	}
	cell_vertices_ = cell_vertices_new_gen;
	
}