/*
 * orbit_tracer.hxx
 *
 *  Created on: 28 de set de 2016
 *      Author: carlosfaruolo
 */

/** A struct to record orbits. */
struct Planetarium::OrbitTracer
{
	OrbitTraceStyle style;
	bool isActive;
	unsigned traceLength;
	std::map<Body2D*, std::deque<Vector2D> > traces;

	/// Creates a orbit tracer that uses the given planetarium as reference for the physics properties.
	OrbitTracer(Planetarium* p)
	: style(LINEAR), isActive(false), traceLength(20), traces(),
	  planetarium(p)
	{}

	/// Record on the queue the given body's current position
	void record(Body2DClone& body, const Vector2D& referenceFramePosition)
	{
		this->traces[body.original].push_back(body.clone.position-referenceFramePosition); //queue push
		while(this->traces[body.original].size() > traceLength)
			this->traces[body.original].pop_front(); //queue pop
	}

	/// Get the trace for the given body. If there is no trace of the given body, a new empty will be created and returned.
	std::deque<Vector2D>& getTrace(Body2D* body)
	{
		return this->traces[body];
	}

	/// Erases the given body's tracing data.
	void clearTrace(const Body2D* body)
	{
		this->traces.erase(const_cast<Body2D*>(body));
	}

	/// Resets the tracer by erasing all tracing data.
	void reset()
	{
		this->traces.clear();
	}

	/// Draws the given body trace on this tracer's planetarium.
	void drawTrace(Body2D* body)
	{
		if(body->userObject == null) return; //if there isn't body data, there isn't body color. leave.
		SDL_Color& bodyColor = static_cast<PlanetariumUserObject*>(body->userObject)->color;

		deque<Vector2D>& trace = this->getTrace(body);

		drawTrace(trace, bodyColor);
	}

	/// Draws the given body trace on this tracer's planetarium.
	void drawTrace(Body2DClone& body)
	{
		if(body.clone.userObject == null) return; //if there isn't body data, there isn't body color. leave.
		SDL_Color& bodyColor = static_cast<PlanetariumUserObject*>(body.clone.userObject)->color;

		deque<Vector2D>& trace = this->getTrace(body.original); //get original body trace

		drawTrace(trace, bodyColor);
	}

	protected:
	Planetarium* planetarium;

	/// Draws the given trace on this tracer's planetarium, with the given color. (usually delegates to other drawXXX method)
	void drawTrace(deque<Vector2D>& trace, const SDL_Color& color)
	{
		switch(style)
		{
			case DOTTED:			drawDotted(trace, color); break;
			default:case LINEAR:	drawLinear(trace, color); break;
			case SPLINE:			drawQuadricBezier(trace, color); break;
		}
	}

	/// Draws the given trace on this tracer's planetarium as dots, each one for each position.
	void drawDotted(deque<Vector2D>& trace, const SDL_Color& bodyColor)
	{
		foreach(Vector2D&, r, deque<Vector2D>, trace)
		{
			Vector2D pv = planetarium->getTransposedNoRef(r);
			pixelRGBA(planetarium->surf, round(pv.x), round(pv.y), bodyColor.r, bodyColor.g, bodyColor.b, 255);
		}
	}

	/// Draws the given trace on this tracer's planetarium as lines, each one for each two positions.
	void drawLinear(deque<Vector2D>& trace, const SDL_Color& bodyColor)
	{
		int (*const line_function) (SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) = (planetarium->tryAA? aalineRGBA : lineRGBA);

		if(trace.empty()) return;
		Vector2D previousPosition = trace.front();
		foreach(Vector2D&, recordedPosition, deque<Vector2D>, trace)
		{
			if(recordedPosition != previousPosition) //avoid drawing segments of same points
			{
				Vector2D recPosTrans = planetarium->getTransposedNoRef(recordedPosition), prevPosTrans = planetarium->getTransposedNoRef(previousPosition);
				line_function(planetarium->surf, round(prevPosTrans.x), round(prevPosTrans.y), round(recPosTrans.x), round(recPosTrans.y), bodyColor.r, bodyColor.g, bodyColor.b, 255);
			}
			previousPosition = recordedPosition;
		}
	}


	// http://stackoverflow.com/questions/9658932/snappy-bezier-curves
	// http://www.ferzkopp.net/Software/SDL_gfx-2.0/Docs/html/_s_d_l__gfx_primitives_8h.html#a7203e3a463da499b5b0cadf211d19ff3
	/// Draws the given trace on this tracer's planetarium as curves, each one for each three positions.
	void drawQuadricBezier(deque<Vector2D>& trace, const SDL_Color& bodyColor)  //still not working properly
	{
		if(trace.empty()) return;
		Vector2D previousPosition = trace.front();
		Vector2D previousSupport;
	//	if(trace.size() > 1) previousSupport = trace.front().times(3).subtract(*(trace.begin()+1)).scale(0.5); //kickstart aux
		if(trace.size() > 1) previousSupport = trace.front().times(2).subtract(*(trace.begin()+1)); //kickstart aux
		foreach(Vector2D&, recordedPosition, deque<Vector2D>, trace)
		{
			if(recordedPosition != previousPosition) //avoid drawing segments of same points
			{
				//fixme Fix quadratic bezier spline implementation
				Vector2D supportPoint = planetarium->getTransposedNoRef(previousSupport);
				Vector2D recPosTrans = planetarium->getTransposedNoRef(recordedPosition), prevPosTrans = planetarium->getTransposedNoRef(previousPosition);
				Sint16 pxs[] = {static_cast<Sint16>(prevPosTrans.x), static_cast<Sint16>(supportPoint.x), static_cast<Sint16>(recPosTrans.x)};
				Sint16 pys[] = {static_cast<Sint16>(prevPosTrans.y), static_cast<Sint16>(supportPoint.y), static_cast<Sint16>(recPosTrans.y)};
				bezierRGBA(planetarium->surf, pxs, pys, 3, 3, bodyColor.r, bodyColor.g, bodyColor.b, 255);
			}
			previousPosition = recordedPosition;
			previousSupport = previousPosition.times(2).subtract(previousSupport);
		}
	}

//	void drawCubicBezier(std::deque<Vector2D>& trace, SDL_Color* color);  //not implemented

};
