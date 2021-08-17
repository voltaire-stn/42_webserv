# include "Form.hpp"

// Parametric constructor (bool)
Form::Form(std::string name, int toSign, int toExec) : _name(name), _toSign(toSign), _toExec(toExec)
{
	this->_isSigned = 0;
	try
	{
		if (toSign > 150 || toExec > 150)
			throw (Form::GradeTooLowException());
		else if (toSign < 1 || toExec < 1)
			throw (Form::GradeTooHighException());
	}
	catch (Form::GradeTooHighException &e)
	{
		std::cout << RED << "Error (construction) : grade must be [>= 1] ... deleting Form ..." <<  C_RES << std::endl;
		delete this;
	}
	catch (Form::GradeTooLowException &e)
	{
		std::cout << RED << "Error (construction) : grade must be [<= 150] ... deleting Form ..." <<  C_RES << std::endl;
		delete this;
	}
	return ;
}

// Copy constructor
Form::Form(const Form& src) : _name(src.getName()), _isSigned(src.getSigned()), _toSign(src.getToSign()), _toExec(src.getToExec())
{
	return;
}

// Destructor
Form::~Form(void)
{
	std::cout << CORAIL << "Form destruction..." << C_RES << std::endl;
	return;
}


// Assignation operator
Form &	Form::operator=(const Form& rhs)
{
	std::cout << GREY << "Form Assignation operator called" << C_RES << std::endl;
	if (this != &rhs)
	{
		this->_isSigned = rhs.getSigned();
	}
	return (*this);
}

// getters and setters (non static attributes)
std::string Form::getName(void) const
{
	return (this->_name);
}

bool Form::getSigned(void) const
{
	return (this->_isSigned);
}

int Form::getToSign(void) const
{
	return (this->_toSign);
}

int Form::getToExec(void) const
{
	return (this->_toExec);
}

void Form::beSigned(Bureaucrat & b)
{
	if (b.signForm(*this) == 0)
		this->_isSigned = 1;
	return ;
}

// overload de <<
std::ostream &operator<<(std::ostream &os, const Form & src)
{
	os << BROWN << "[" << src.getName() << "]";
	if (src.getSigned() == 0)
		os << ORANGE << " [not signed yet]" << C_RES;
	else
		os << GREEN_TREE << " [already signed]" << C_RES;
	os	<< BROWN << " can be signed from grade [" << src.getToSign() << "] and executed from grade [" << src.getToExec() << "]" << C_RES;
	return (os);
}

void		Form::execute(Bureaucrat const & executor) const
{
	try
	{
		if (executor.getGrade() > this->_toExec)
			throw (Form::GradeTooLowException());
		if (this->getSigned() == 0)
			throw (std::exception());
		else
			this->action();
	}
	catch (Form::GradeTooLowException &e)
	{
		std::cout << RED << "Error (execution) : " << executor.getName() << " (grade required to execute " << this->_name << ": " << this->_toExec << ", actual grade : " << executor.getGrade() << ")" << C_RES << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << RED << "Error (execution) : form " << this->_name << " has not been signed yet" << C_RES << std::endl;
	}
	return;
}