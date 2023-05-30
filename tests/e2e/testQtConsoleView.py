#!/usr/bin/env python3
import pytest
import os

@pytest.fixture
def example_fixture():
  sourcetrail_exec  = ""
  sourcetrail_index = ""
  assert os.path.exists(sourcetrail_exec), ""
  assert os.path.exists(sourcetrail_index), ""

def test_with_fixture(example_fixture):
  assert True