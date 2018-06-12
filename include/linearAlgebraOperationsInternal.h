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

#ifndef linearAlgebraOperationsInternal_h
#define linearAlgebraOperationsInternal_h

#include <headers.h>

namespace dftfe
{

  namespace linearAlgebraOperations
  {
    /** @file linearAlgebraOperationsInternal.h
     *  @brief Contains small internal functions used in linearAlgebraOperations
     *
     *  @author Sambit Das
     */
    namespace internal
    {
#ifdef WITH_SCALAPACK
	/** @brief Wrapper function to create a two dimensional processor grid for a square matrix in
	 * dealii::ScaLAPACKMatrix storage format.
	 *
	 */
	void createProcessGridSquareMatrix(const MPI_Comm & mpi_communicator,
		                           const unsigned size,
				           const unsigned int rowsBlockSize,
					   std::shared_ptr< const dealii::Utilities::MPI::ProcessGrid>  & processGrid);

	/** @brief S=X^{T}*X. X^{T} is the subspaceVectorsArray in the column major format. S is the
	 * overlapMatPar.
	 *
	 * The overlap matrix computation and filling is done in a blocked approach
	 * which avoids creation of full serial overlap matrix memory, and also avoids creation
	 * of another full X memory.
	 *
	 */
	template<typename T>
	void fillParallelOverlapMatrix(const dealii::parallel::distributed::Vector<T> & X,
		                       const unsigned int numberVectors,
		                       const std::shared_ptr< const dealii::Utilities::MPI::ProcessGrid>  & processGrid,
				       dealii::ScaLAPACKMatrix<T> & overlapMatPar);

	/** @brief X^{T}=Q*X^{T}. X^{T} is the subspaceVectorsArray in the column major
	 * format. Q is rotationMatPar.
	 *
	 * The subspace rotation inside this function is done in a blocked approach
	 * which avoids creation of full serial rotation matrix memory, and also avoids creation
	 * of another full subspaceVectorsArray memory.
	 *
	 */
	template<typename T>
	void subspaceRotation(dealii::parallel::distributed::Vector<T> & subspaceVectorsArray,
		              const unsigned int numberSubspaceVectors,
		              const std::shared_ptr< const dealii::Utilities::MPI::ProcessGrid>  & processGrid,
			      const dealii::ScaLAPACKMatrix<T> & rotationMatPar,
			      const bool rotationMatTranspose=false);

#endif
    }
  }
}
#endif
