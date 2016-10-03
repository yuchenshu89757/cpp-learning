
```
class Point{
public:
	Point(float x = 0.0, float y = 0.0);
	Point(const Point &);
	Point &operator=(const Point &);

	float x()const{return _x;}
	float y()const{return _y;}

	virtual ~Point();
	virtual float z(){return 0.0;}
protected:
	float _x, _y;
};
//no inheritance
Point* Point::Point(Point *this, float x, float y)
{
	//set the virtual table pointer(vptr)
	this->_vptr_Point = _vtbl_Point;

	//expand member initialization list
	this->_x = x;
	this->_y = y;

	//return this object
	return this;
}

class Point3d : public virtual Point{
public:
	Point3d(float x = 0.0, float y = 0.0, float z = 0.0) : Point(x, y), _z(z){}
	Point3d(const Point3d &rhs) : Point(rhs), _z(rhs._z){}
	Point3d &operator=(const Point3d &);
	~Point3d();

	virtual float z(){return _z;}
protected:
	float _z;
};
//single inheritance
Point3d* Point3d::Point3d(Point3d *this, float x, float y, float z)
{
	this->Point::Point(x, y);
	
	this->_vptr_Point3d       = _vtbl_Point3d;

	this->_z = z;
	return this;
}
//multiple inheritance
Point3d* Point3d::Point3d(Point3d *this, bool _most_derived, float x, float y, float z)
{
	if(_most_derived)
		this->Point::Point(x, y);

	this->_vptr_Point3d       = _vtbl_Point3d;
	this->_vptr_Point3d_Point = _vtbl_Point3d_Point;

	this->_z = z;
	return this;
}

class Vertex : public virtual Point{...};
class Vertex3d : public Point3d, public Vertex{...};
class PVertex : public Vertex3d{...}; 

Vertex3d* Vertex3d::Vertex3d(Vertex3d *this, bool _most_derived, float x, float y, float z)
{
	if(_most_derived)
		this->Point::Point(x, y);

	//set the _most_derived false and call the front base calsses
	this->Point3d::Point3d(false, x, y, z);
	this->Vertex::Vertex(false, x, y);

	//set vptrs
	this->_vptr_Vertex3d 	   = _vbtl_Vertex3d;
	this->_vptr_Vertex3d_Point = _vbtl_Vertex3d_Point;
	//insert user code

	return this;
}
```
