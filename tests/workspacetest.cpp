/*************************************************************************

  Copyright 2011-2015 Ibrahim Sha'ath

  This file is part of LibKeyFinder.

  LibKeyFinder is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LibKeyFinder is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LibKeyFinder.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************/

#include "_testhelpers.h"

TEST(WorkspaceTest, ConstructorDefaultsWork)
{
    KeyFinder::Workspace w;

    ASSERT_EQ(0, w.preprocessedBuffer.getChannels());
    ASSERT_EQ(0, w.preprocessedBuffer.getFrameRate());
    ASSERT_EQ(0, w.preprocessedBuffer.getSampleCount());

    ASSERT_EQ(0, w.remainderBuffer.getChannels());
    ASSERT_EQ(0, w.remainderBuffer.getFrameRate());
    ASSERT_EQ(0, w.remainderBuffer.getSampleCount());

    ASSERT_EQ(NULL, w.chromagram);
    ASSERT_EQ(NULL, w.fftAdapter);
    ASSERT_EQ(NULL, w.lpfBuffer);
}
