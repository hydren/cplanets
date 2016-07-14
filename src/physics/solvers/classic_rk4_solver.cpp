/*
 * classic_rk4_solver.cpp
 *
 *  Created on: 14 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "classic_rk4_solver.hpp"

#include <map>
using std::map;

#include <vector>
using std::vector;

DEFINE_CLASS_FACTORY(ClassicRk4Solver, "Runge-Kutta (4th order, classic)");

ClassicRk4Solver::ClassicRk4Solver(Universe2D& u)
: AbstractPhysics2DSolver(&CLASS_FACTORY, u, 0.01)
{}

void ClassicRk4Solver::step()
{
	const double one_sixth_timestep = timestep / 6.0;
	map<Body2D*, Vector2D> vn_, rn_;

	computeAccelerations();

	foreach(Body2D*, body, vector<Body2D*>, universe.bodies)
	{
		vn_[body] = body->velocity + body->acceleration * one_sixth_timestep;
		rn_[body] = body->position + (body->velocity + body->velocity) * one_sixth_timestep;
	}


	timeElapsed += timestep;
}

/*

Theory / Drafts

k1v = a(rn)
k1r = vn
k2v = a(rn + k1r*h/2) = a(rn + 0.5*h*k1r) = a(rn + 0.5*h*vn)
k2r = vn + k1v*h/2 = vn + 0.5*h*k1v = vn + 0.5*h*a(rn)
k3v = a(rn + k2r*h/2) = a(rn + 0.5*h*k2r) = a(rn + 0.5*h*(vn + 0.5*h*a(rn))) = a(rn + 0.5*h*vn + 0.25*h*h*a(rn)))
k3r = vn + k2v*h/2 = vn + 0.5*h*k2v = vn + 0.5*h*a(rn + 0.5*h*vn)
k4v = a(rn + k3r*h) = a(rn + h*k3r) = a(rn + h*(vn + 0.5*h*a(rn + 0.5*h*vn))) = a(rn + h*vn + 0.5*h*h*a(rn + 0.5*h*vn)))
k4r = vn + k3v*h = vn + h*k3v = vn + h*a(rn + 0.5*h*vn + 0.25*h*h*a(rn)))

vn2 = vn + (k1v + 2*k2v + 2*k3v + k4v) * h/6
rn2 = rn + (k1r + 2*k2r + 2*k3r + k4r) * h/6

vn2 = vn + (a(rn) + 2*a(rn + 0.5*h*vn) + 2*a(rn + 0.5*h*vn + 0.25*h*h*a(rn)) + a(rn + h*vn + 0.5*h*h*a(rn + 0.5*h*vn))) * h/6
rn2 = rn + (vn + 2*(vn + 0.5*h*a(rn)) + 2*(vn + 0.5*h*a(rn + 0.5*h*vn)) + (vn + h*a(rn + 0.5*h*vn + 0.25*h*h*a(rn)))) * h/6

vn2 = vn +                    (a(rn) +              2*a(rn + 0.5*h*vn) +        2*a(rn + 0.5*h*vn + 0.25*h*h*a(rn)) + a(rn + h*vn + 0.5*h*h*a(rn + 0.5*h*vn))) * h/6
rn2 = rn + (vn + 2*(vn + 0.5*h*a(rn)) + 2*(vn + 0.5*h*a(rn + 0.5*h*vn)) + (vn + h*a(rn + 0.5*h*vn + 0.25*h*h*a(rn))))) * h/6

a0 = a(rn) = a(rn0)
a1 = a(rn + 0.5*h*vn) = a(rn1)
a2 = a(rn + 0.5*h*vn + 0.25*h*h*a(rn)) = a(rn2)
a3 = a(rn + h*vn + 0.5*h*h*a(rn + 0.5*h*vn)) = a(rn3)

// fixme afterwards here is wrong:

rn0 = rn
rn1 = rn + 0.5*h*vn
rn2 = rn + 0.25*h*h*vn*a0
rn3 = rn + 0.5*h*h*vn*a1

rn + 0.5*h*vn + J = rn + 0.25*h*h*vn*a0
J = rn + 0.25*h*h*vn*a0 - rn + 0.5*h*vn = 0.25*h*h*vn*a0 - 0.5*h*vn = vn*h(0.25*h*a0 - 0.5)

rn + 0.25*h*h*vn*a0 + K = rn + 0.5*h*h*vn*a1
K = 0.5*h*h*vn*a1 - 0.25*h*h*vn*a0 = h*h*vn(0.5*a1 - 0.25*a0)

rn0 = rn
rn1 = rn0 + 0.5*h*vn
rn2 = rn1 + vn*h(0.25*h*a0 - 0.5)
rn3 = rn2 + h*h*vn(0.5*a1 - 0.25*a0)

rn0 = rn
rn1 = rn0 + 0.5*h*vn
rn2 = rn1 + vn*h(0.25*h*a(rn0) - 0.5)
rn3 = rn2 + h*h*vn(0.5*a(rn1) - 0.25*a(rn0))

vn_ = vn + (a(rn0) + 2*a(rn1) + 2*a(rn2) + a(rn3)) * h/6
rn_ = rn + (vn + 2*0.5*h*vn*a(rn0) + 2*0.5*h*vn*a(rn1) + vn*h*a(rn2)) * h/6



// vn2 = vn
vn_ += (h/6) * a(rn0)
vn_ += (h/6) * 2*a(rn1)
vn_ += (h/6) * 2*a(rn2)
vn_ += (h/6) * a(rn3)


// rn2 = rn
rn_ += (h/6) * vn
rn_ += (h/6) * 2*0.5*h*vn*a(rn0)
rn_ += (h/6) * 2*0.5*h*vn*a(rn1)
rn_ += (h/6) * vn*h*a(rn2)


//algorithm

// vn_ = vn
// rn_ = rn

r = rn0 = rn
a = a(r) // a(rn0)

vn_ += (h/6) * a
rn_ += (h/6) * vn
rn_ += (h/6) * 2*0.5*h*vn*a

aux_arn0 = a  //a(rn0)
r += 0.5*h*vn // r = rn1 = rn0 + 0.5*h*vn
a = a(r) // a(rn1)

vn_ += (h/6) * 2*a
rn_ += (h/6) * 2*0.5*h*vn*a

aux_arn1 = a  //a(rn1)
r += vn*h(0.25*h*aux_arn0 - 0.5) // r = rn2 = rn1 + vn*h(0.25*h*aux_arn0 - 0.5)
a = a(r) // a(rn2)

vn_ += (h/6) * 2*a
rn_ += (h/6) * vn*h*a

r += h*h*vn(0.5*aux_arn1 - 0.25*aux_arn0) // r = rn3 = rn2 + h*h*vn(0.5*aux_arn1 - 0.25*aux_arn0)
a = a(r) // a(rn3)

vn_ += (h/6) * a

r = rn_
v = vn_
a = a(rn_) //optional



 */
