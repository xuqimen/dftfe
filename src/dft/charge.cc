// ---------------------------------------------------------------------
//
// Copyright (c) 2017-2018 The Regents of the University of Michigan and DFT-FE authors.
//
// This file is part of the DFT-FE code.
//
// The DFT-FE code is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the DFT-FE distribution.
//
// ---------------------------------------------------------------------
//
// @author Shiva Rudraraju, Phani Motamarri
//

//source file for all charge calculations

//
//compute total charge using quad point values
//
template <unsigned int FEOrder>
double dftClass<FEOrder>::totalCharge(const dealii::DoFHandler<3> & dofHandlerOfField,
				      const std::map<dealii::CellId, std::vector<double> > *rhoQuadValues)
{
  double normValue = 0.0;
  QGauss<3>  quadrature_formula(C_num1DQuad<FEOrder>());
  FEValues<3> fe_values (dofHandlerOfField.get_fe(), quadrature_formula, update_JxW_values);
  const unsigned int dofs_per_cell = dofHandlerOfField.get_fe().dofs_per_cell;
  const unsigned int n_q_points    = quadrature_formula.size();

  DoFHandler<3>::active_cell_iterator
    cell = dofHandlerOfField.begin_active(),
    endc = dofHandlerOfField.end();
  for (; cell!=endc; ++cell) {
    if (cell->is_locally_owned()){
      fe_values.reinit (cell);
      for (unsigned int q_point=0; q_point<n_q_points; ++q_point){
        normValue+=(*rhoQuadValues).find(cell->id())->second[q_point]*fe_values.JxW(q_point);
      }
    }
  }
  return Utilities::MPI::sum(normValue, mpi_communicator);
}


//
//compute total charge using nodal point values 
//
template <unsigned int FEOrder>
double dftClass<FEOrder>::totalCharge(const dealii::DoFHandler<3> & dofHandlerOfField,
				      const vectorType & rhoNodalField)
{
  double normValue = 0.0;
  QGauss<3>  quadrature_formula(C_num1DQuad<FEOrder>());
  FEValues<3> fe_values (dofHandlerOfField.get_fe(), quadrature_formula, update_values | update_JxW_values);
  const unsigned int dofs_per_cell = dofHandlerOfField.get_fe().dofs_per_cell;
  const unsigned int n_q_points    = quadrature_formula.size();

  DoFHandler<3>::active_cell_iterator
    cell = dofHandlerOfField.begin_active(),
    endc = dofHandlerOfField.end();
  for(; cell!=endc; ++cell) 
    {
      if(cell->is_locally_owned())
	{
	  fe_values.reinit (cell);
	  std::vector<double> tempRho(n_q_points);
	  fe_values.get_function_values(rhoNodalField,tempRho);
	  for (unsigned int q_point=0; q_point<n_q_points; ++q_point)
	    {
	      normValue += tempRho[q_point]*fe_values.JxW(q_point);
	    }
	}
    }
  return Utilities::MPI::sum(normValue, mpi_communicator);
}

//
//compute total charge using nodal point values by filling the quadrature point values of the nodal field
//
template <unsigned int FEOrder>
double dftClass<FEOrder>::totalCharge(const dealii::DoFHandler<3> & dofHandlerOfField,
				      const vectorType & rhoNodalField,
				      std::map<dealii::CellId,std::vector<double> > & rhoQuadValues)
{
  double normValue = 0.0;
  QGauss<3>  quadrature_formula(C_num1DQuad<FEOrder>());
  FEValues<3> fe_values (dofHandlerOfField.get_fe(), quadrature_formula, update_values | update_JxW_values);
  const unsigned int dofs_per_cell = dofHandlerOfField.get_fe().dofs_per_cell;
  const unsigned int n_q_points    = quadrature_formula.size();
  std::vector<double> tempRho(n_q_points);

  DoFHandler<3>::active_cell_iterator
    cell = dofHandlerOfField.begin_active(),
    endc = dofHandlerOfField.end();
  for(; cell!=endc; ++cell) 
    {
      if(cell->is_locally_owned())
	{
	  fe_values.reinit (cell);
	  fe_values.get_function_values(rhoNodalField,tempRho);
	  rhoQuadValues[cell->id()].resize(n_q_points);
	  for (unsigned int q_point=0; q_point<n_q_points; ++q_point)
	    {
	      rhoQuadValues[cell->id()][q_point] = tempRho[q_point];
	      normValue += tempRho[q_point]*fe_values.JxW(q_point);
	    }
	}
    }
  return Utilities::MPI::sum(normValue, mpi_communicator);
}


//compute total charge
template <unsigned int FEOrder>
double dftClass<FEOrder>::totalMagnetization(const std::map<dealii::CellId, std::vector<double> > *rhoQuadValues){
  double normValue=0.0;
  QGauss<3>  quadrature_formula(C_num1DQuad<FEOrder>());
  FEValues<3> fe_values (FE, quadrature_formula, update_JxW_values);
  const unsigned int   dofs_per_cell = FE.dofs_per_cell;
  const unsigned int   n_q_points    = quadrature_formula.size();

  DoFHandler<3>::active_cell_iterator
    cell = dofHandler.begin_active(),
    endc = dofHandler.end();
  for (; cell!=endc; ++cell) {
    if (cell->is_locally_owned()){
      fe_values.reinit (cell);
      for (unsigned int q_point=0; q_point<n_q_points; ++q_point){
        normValue+=((*rhoQuadValues).find(cell->id())->second[2*q_point]-(*rhoQuadValues).find(cell->id())->second[2*q_point+1])*fe_values.JxW(q_point);
      }
    }
  }
  return Utilities::MPI::sum(normValue, mpi_communicator);
}
